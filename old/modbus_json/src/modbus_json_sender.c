/*
 * modbus_json_sender.c
 *
 *  Created on: 10 Sep 2013
 *      Author: rnicolau
 */

#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <pthread.h>
#include <jansson.h>

#include "curl/curl.h"
#include "modbus_json.h"

volatile int modbus_json_sender_loop_on = 1;

//backup file related
typedef struct SHARED_DATA
{
  CURL* curl_handle;
  FILE* file_w;
  FILE* file_r;
  pthread_mutex_t file_mode_mutex;
  pthread_t thread;
  volatile int in_file_mode;
  int dummy_writes;
}SHARED_DATA;

void set_file_mode(int mode, SHARED_DATA* shd)
{
  pthread_mutex_lock(&shd->file_mode_mutex);
  shd->in_file_mode = mode;
  pthread_mutex_unlock(&shd->file_mode_mutex);
}

int get_file_mode(SHARED_DATA* shd)
{
  int mode;
  pthread_mutex_lock(&shd->file_mode_mutex);
  mode = shd->in_file_mode;
  pthread_mutex_unlock(&shd->file_mode_mutex);
  return mode;
}

void* backup_sender_thread(void* args)
{
  SHARED_DATA* sh_data = (SHARED_DATA*)args;
  char buffer[1024];
  size_t old_pos = ftell(sh_data->file_r);
  clearerr(sh_data->file_r);

  INFO("Switching to file mode\n");

  while(!feof(sh_data->file_r) && !ferror(sh_data->file_r))
  {
    json_error_t error;
    json_t* json_object = json_loadf(sh_data->file_r, JSON_DECODE_ANY | JSON_DISABLE_EOF_CHECK, &error);
    char* json_string = NULL;
    if (json_object == NULL)
    {
      break;
    }
    else
    {
      int send_succeeded = 0;

      //advance 1 newline
      fseek(sh_data->file_r, 1, SEEK_CUR);

      //get next line to check for sent / not sent
      if(!feof(sh_data->file_r)
        && !ferror(sh_data->file_r)
        && fgets(buffer, sizeof(buffer), sh_data->file_r) != NULL)
      {
        if (strncmp(buffer, JSON_SENT_TOKEN, strlen(JSON_SENT_TOKEN)) == 0)
        {
          //was sent, continue
          old_pos = ftell(sh_data->file_r);
          continue;
        }
      }

      //try to send the data
      json_string = json_dumps(json_object, JSON_PRESERVE_ORDER | JSON_INDENT(1));
      if (sh_data->dummy_writes)
      {
        printf(json_string);
        send_succeeded = 1;
      }
      else
      {
        curl_easy_setopt(sh_data->curl_handle, CURLOPT_CONNECTTIMEOUT, CURL_TIMEOUT);
        curl_easy_setopt(sh_data->curl_handle, CURLOPT_TIMEOUT, CURL_TIMEOUT);
        curl_easy_setopt(sh_data->curl_handle, CURLOPT_COPYPOSTFIELDS, json_string);
        CURLcode curl_result = curl_easy_perform(sh_data->curl_handle);
        if (curl_result != CURLE_OK)
        {
          ERR("curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_result));
        }
        else
        {
          INFO("curl sent OK");
          send_succeeded = 1;
        }
      }

      if (send_succeeded)
      {
        //advance to current pos
        old_pos = ftell(sh_data->file_r);
        //mark as sent
        fseek(sh_data->file_r, -strlen(JSON_SENT_TOKEN), SEEK_CUR);
        fwrite(JSON_SENT_TOKEN, strlen(JSON_SENT_TOKEN), 1, sh_data->file_r);

      }
      else
      {
        //rewind
        fseek(sh_data->file_r, old_pos, SEEK_SET);
        //and wait one sec
        sleep(1);
      }

      free(json_string);
    }
  }

  INFO("Switching off file mode\n");
  set_file_mode(0, sh_data);
  return NULL;
}


char* convert_to_json(ENERGY_METER_PACKET_HEADER* header, ENERGY_METER_PACKET_DATA* data)
{
  char* json_string = NULL;
  json_t* json_object = NULL;

  switch (header->type)
  {
  case ENERGY_METER_PACK_TYPE_ENERGY:
    {
      json_object = json_pack(
          "{s:s s:i"
          "s:i}",
          "TimeStamp",  header->timestamp,
          "id",         header->id,
          "energy",     data->energy[0]
       );
    }
    break;
  case ENERGY_METER_PACK_TYPE_DETAILS:
    {
      json_object = json_pack(
          "{s:s s:i"
          "s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i s:i}",
          "TimeStamp",    header->timestamp,
          "id",           header->id,
          "voltage1",     data->details[VOLTAGE_1],
          "voltage2",     data->details[VOLTAGE_2],
          "voltage3",     data->details[VOLTAGE_3],
          "voltage13",    data->details[VOLTAGE_12],
          "voltage23",    data->details[VOLTAGE_23],
          "voltage31",    data->details[VOLTAGE_31],
          "current1",     data->details[CURRENT_1],
          "current2",     data->details[CURRENT_2],
          "current3",     data->details[CURRENT_3],
          "actPower1",    data->details[ACT_POWER_1],
          "actPower2",    data->details[ACT_POWER_2],
          "actPower3",    data->details[ACT_POWER_3],
          "reactPower1",  data->details[REACT_POWER_1],
          "reactPower2",  data->details[REACT_POWER_2],
          "reactPower3",  data->details[REACT_POWER_3],
          "currentN",     data->details[CURRENT_N],
          "frequency",    data->details[FREQUENCY],
          "appCurrent1",  data->details[APP_POWER_1],
          "appCurrent2",  data->details[APP_POWER_2],
          "appCurrent3",  data->details[APP_POWER_3]
      );
    }
    break;
  case ENERGY_METER_PACK_TYPE_ERROR:
    {
      json_object = json_pack(
          "{s:s s:i"
          "s:s}",
          "TimeStamp",    header->timestamp,
          "id",           header->id,
          "ModbusError", data->error
       );
      printf("%s", data->error);
    }
    break;
  default:
    ERR("Invalid energy info packet\n");
    exit(EXIT_FAILURE);
    break;
  }

  if(json_object)
  {
    json_string = json_dumps(json_object, JSON_PRESERVE_ORDER | JSON_INDENT(1));
  }
  else
  {
    CRIT("json packing failed\n");
  }
  return json_string;
}

int modbus_json_sender_loop(FILE *stream, char* energy_url, char* error_url, int dummy_writes)
{
  //file mode backup indicator
  int switch_to_file_mode = 0;

  //buffer file
  SHARED_DATA sh_data = {0};

  //json buffer
  char* json_buffer = NULL;

  //open write file, same file, one r one w
  {
    sh_data.file_w = fopen(JSON_BUFFER_FILE_NAME, "a");
    sh_data.file_r = fopen(JSON_BUFFER_FILE_NAME, "r+");
    sh_data.in_file_mode = 0;
    sh_data.dummy_writes = dummy_writes;
    if (sh_data.file_w == NULL || sh_data.file_r == NULL)
    {
      CRIT("Failed to open buffer file\n");
      exit(EXIT_FAILURE);
    }
  }

  //Initialise curl
  {
    curl_global_init(CURL_GLOBAL_ALL);
    sh_data.curl_handle = curl_easy_init();
    if (sh_data.curl_handle == NULL)
    {
      CRIT("Failed to initialise libcurl\n");
      exit(EXIT_FAILURE);
    }
  }

  while (modbus_json_sender_loop_on)
  {
    ENERGY_METER_PACKET_HEADER header;
    ENERGY_METER_PACKET_DATA data;
    size_t read_size = 0;

    //check if enough data
    {
      struct pollfd poll_fd;
      poll_fd.fd = fileno(stream);
      poll_fd.events = POLLIN;
      poll_fd.revents = 0;
      int poll_res = poll(&poll_fd, 1, 0);
      if (poll_res > 0)
      {
        read_size = fread(&header, sizeof(ENERGY_METER_PACKET_HEADER), 1, stream);
        if (read_size)
        {
          read_size = fread(&data, header.size, 1, stream);
        }
      }
    }

    if (read_size)
    {
      //remove extra new line
      header.timestamp[strlen(header.timestamp) - 1] = 0;
      json_buffer = convert_to_json(&header, &data);

      if (!get_file_mode(&sh_data))
      {
        if (dummy_writes)
        {
          printf(json_buffer);
          printf("\n");
        }
        else
        {
          if (header.type == ENERGY_METER_PACK_TYPE_ERROR)
          {
            curl_easy_setopt(sh_data.curl_handle, CURLOPT_URL, error_url);
          }
          else
          {
            curl_easy_setopt(sh_data.curl_handle, CURLOPT_URL, energy_url);
          }
          curl_easy_setopt(sh_data.curl_handle, CURLOPT_CONNECTTIMEOUT, CURL_TIMEOUT);
          curl_easy_setopt(sh_data.curl_handle, CURLOPT_TIMEOUT, CURL_TIMEOUT);
          curl_easy_setopt(sh_data.curl_handle, CURLOPT_COPYPOSTFIELDS, json_buffer);
          CURLcode curl_result = curl_easy_perform(sh_data.curl_handle);
          if (curl_result != CURLE_OK)
          {
            switch_to_file_mode = 1;
            ERR("curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_result));
          }
          else
          {
            INFO("curl sent OK");
          }
        }
      }

      if (get_file_mode(&sh_data) || switch_to_file_mode)
      {
        if (   fwrite(json_buffer, strlen(json_buffer), 1, sh_data.file_w) != 1
            || fwrite("\n", strlen("\n"), 1, sh_data.file_w) != 1
            || fwrite(JSON_NOTSENT_TOKEN, strlen(JSON_NOTSENT_TOKEN), 1, sh_data.file_w) != 1)
        {
          CRIT("Failed to write to buffer file\n");
          exit(EXIT_FAILURE);
        }
        fflush(sh_data.file_w);
      }

      free(json_buffer);
    }

    //spawn a thread that tries to send the backup file contents
    if (switch_to_file_mode)
    {
      switch_to_file_mode = 0;
      set_file_mode(1, &sh_data);
      pthread_create(&sh_data.thread, NULL, backup_sender_thread, &sh_data);
    }

    usleep(1000 * JSON_SENDER_LOOP_CLOCK);
  }



  //cleanup curl
  curl_easy_cleanup(sh_data.curl_handle);
  curl_global_cleanup();

  //free json buffer
  free(json_buffer);

  //close files
  fclose(sh_data.file_r);
  fclose(sh_data.file_w);

  return 0;
}

