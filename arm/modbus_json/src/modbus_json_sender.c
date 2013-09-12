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

#include "curl/curl.h"
#include "modbus_json.h"

volatile int modbus_json_sender_loop_on = 1;

//backup file related
typedef struct THREAD_ARGS
{
  CURL** curl_handle;
  FILE** file;
  int dummy_writes;
}THREAD_ARGS;
THREAD_ARGS g_th_args;
pthread_mutex_t g_file_mode_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t g_thread;
volatile int g_in_file_mode = 0;

void set_file_mode(int mode)
{
  pthread_mutex_lock(&g_file_mode_mutex);
  g_in_file_mode = mode;
  pthread_mutex_unlock(&g_file_mode_mutex);
}

int get_file_mode()
{
  int mode;
  pthread_mutex_lock(&g_file_mode_mutex);
  mode = g_in_file_mode;
  pthread_mutex_unlock(&g_file_mode_mutex);
  return mode;
}

void* backup_sender_thread(void* args)
{
  THREAD_ARGS th_args = *(THREAD_ARGS*)args;
  FILE* buffer_file_r = *th_args.file;
  char buffer[1024];
  int max_json_size = 10 * 1024;
  char* json_buffer = malloc(max_json_size);
  size_t old_pos = ftell(buffer_file_r);

  if (json_buffer == NULL)
  {
    fprintf(stderr, "Allocation failed\n");
    exit(EXIT_FAILURE);
  }

  printf("switching to file mode\n");

  while(!feof(buffer_file_r)
      && !ferror(buffer_file_r)
      && fgets(buffer, sizeof(buffer), buffer_file_r) != NULL)
  {

    //end of JSON received, post data
    if (strncmp(buffer, JSON_STOP_TOKEN, strlen(JSON_STOP_TOKEN)) == 0)
    {

      //get next line to check for sent / not sent
      if(!feof(buffer_file_r)
        && !ferror(buffer_file_r)
        && fgets(buffer, sizeof(buffer), buffer_file_r) != NULL)
      {
        if (strncmp(buffer, JSON_SENT_TOKEN, strlen(JSON_SENT_TOKEN)) == 0)
        {
          //was sent, continue
          old_pos = ftell(buffer_file_r);
          continue;
        }
      }

      //read and send the data
      {
        //get size
        size_t curr_pos = ftell(buffer_file_r);
        size_t size = curr_pos - old_pos;
        int send_succeeded = 0;

        //increase buffer
        if (size > max_json_size)
        {
          max_json_size = 2 * 1024 + size;
          free (json_buffer);
          json_buffer = malloc(max_json_size);
          if (json_buffer == NULL)
          {
            fprintf(stderr, "Allocation failed\n");
            exit(EXIT_FAILURE);
          }
        }

        fseek(buffer_file_r, old_pos, SEEK_SET);
        fread(json_buffer, size, 1, buffer_file_r);
        json_buffer[size - strlen(JSON_SENT_TOKEN) - strlen(JSON_STOP_TOKEN)] = 0;

        if (th_args.dummy_writes)
        {
          printf(json_buffer);
          send_succeeded = 1;
        }
        else
        {
          curl_easy_setopt(*th_args.curl_handle, CURLOPT_COPYPOSTFIELDS, json_buffer);
          CURLcode curl_result = curl_easy_perform(*th_args.curl_handle);
          if (curl_result != CURLE_OK)
          {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(curl_result));
          }
          else
          {
            send_succeeded = 1;
          }
        }

        if (send_succeeded)
        {
          //advance to current pos
          old_pos = ftell(buffer_file_r);
          //mark as sent
          fseek(buffer_file_r, -strlen(JSON_SENT_TOKEN), SEEK_CUR);
          fwrite(JSON_SENT_TOKEN, strlen(JSON_SENT_TOKEN), 1, buffer_file_r);

        }
        else
        {
          //rewind
          fseek(buffer_file_r, old_pos, SEEK_SET);
          //and wait one sec
          sleep(1);
        }
      }

    }
  }

  printf("switching off file mode\n");
  set_file_mode(0);
  return NULL;
}


void convert_to_json(char* json_buffer, ENERGY_METER_PACKET* energy_packet, size_t max_size)
{
  int count = 0;
  switch (energy_packet->type)
  {
  case ENERGY_METER_PACK_TYPE_ENERGY:
    {
      count = snprintf(json_buffer, max_size,
          "{\n"
          "\"TimeStamp\":\"%s\",\n"
          "\"id\":%lu,\n"
          "\"energy\":%u\n"
          "}\n",
          energy_packet->timestamp,
          energy_packet->id,
          energy_packet->payload.tab_reg[0]
          );
    }
    break;
  case ENERGY_METER_PACK_TYPE_DETAILS:
    {
      count = snprintf(json_buffer, max_size,
          "{\n"
          "\"TimeStamp\":\"%s\",\n"
          "\"id\":%lu,\n"
          "\"voltage1\":%u,\n"
          "\"voltage2\":%u,\n"
          "\"voltage3\":%u,\n"
          "\"voltage13\":%u,\n"
          "\"voltage23\":%u,\n"
          "\"voltage31\":%u,\n"
          "\"current1\":%u,\n"
          "\"current2\":%u,\n"
          "\"current3\":%u,\n"
          "\"actPower1\":%u,\n"
          "\"actPower2\":%u,\n"
          "\"actPower3\":%u,\n"
          "\"reactPower1\":%u,\n"
          "\"reactPower2\":%u,\n"
          "\"reactPower3\":%u,\n"
          "\"currentN\":%u,\n"
          "\"frequency\":%u,\n"
          "\"appCurrent1\":%u,\n"
          "\"appCurrent2\":%u,\n"
          "\"appCurrent3\":%u\n"
          "}\n",
          energy_packet->timestamp,
          energy_packet->id,
          energy_packet->payload.tab_reg[VOLTAGE_1],
          energy_packet->payload.tab_reg[VOLTAGE_2],
          energy_packet->payload.tab_reg[VOLTAGE_3],
          energy_packet->payload.tab_reg[VOLTAGE_12],
          energy_packet->payload.tab_reg[VOLTAGE_23],
          energy_packet->payload.tab_reg[VOLTAGE_31],
          energy_packet->payload.tab_reg[CURRENT_1],
          energy_packet->payload.tab_reg[CURRENT_2],
          energy_packet->payload.tab_reg[CURRENT_3],
          energy_packet->payload.tab_reg[ACT_POWER_1],
          energy_packet->payload.tab_reg[ACT_POWER_2],
          energy_packet->payload.tab_reg[ACT_POWER_3],
          energy_packet->payload.tab_reg[REACT_POWER_1],
          energy_packet->payload.tab_reg[REACT_POWER_2],
          energy_packet->payload.tab_reg[REACT_POWER_3],
          energy_packet->payload.tab_reg[CURRENT_N],
          energy_packet->payload.tab_reg[FREQUENCY],
          energy_packet->payload.tab_reg[APP_POWER_1],
          energy_packet->payload.tab_reg[APP_POWER_2],
          energy_packet->payload.tab_reg[APP_POWER_3]
          );
    }
    break;
  default:
    fprintf(stderr, "Invalid energy info packet\n");
    exit(EXIT_FAILURE);
    break;
  }

  if (count < 0)
  {
    fprintf(stderr, "Failed to format json data using snprintf: %s\n",
        strerror(errno));
    exit(EXIT_FAILURE);
  }
  if (max_size <= count)
  {
    fprintf(stderr,
        "Buffer is not big enough (%u) to format json data. %s\n",
        (int) max_size, strerror(errno));
    exit(EXIT_FAILURE);
  }
}

int modbus_json_sender_loop(FILE *stream, char* URL, int dummy_writes)
{
  //buffers and helpers
  size_t json_buffer_size = 2 * 1024;
  char *json_buffer = malloc(json_buffer_size);

  //curl data
  CURL* curl_handle;
  CURLcode curl_result = CURLE_OK;

  //file mode backup indicator
  int switch_to_file_mode = 0;

  //buffer file
  FILE *buffer_file_r, *buffer_file_w;

  //open write file, same file, one r one w
  {
    buffer_file_w = fopen(JSON_BUFFER_FILE_NAME, "a");
    buffer_file_r = fopen(JSON_BUFFER_FILE_NAME, "r+");
    g_in_file_mode = 0;
    if (buffer_file_w == NULL || buffer_file_r == NULL)
    {
      fprintf(stderr, "Failed to open buffer file\n");
      exit(EXIT_FAILURE);
    }
  }

  //Initialise curl
  {
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
    if (curl_handle == NULL)
    {
      fprintf(stderr, "Failed to initialise libcurl\n");
      exit(EXIT_FAILURE);
    }
    else
    {
      curl_easy_setopt(curl_handle, CURLOPT_URL, URL);
    }
  }

  while (modbus_json_sender_loop_on)
  {
    ENERGY_METER_PACKET energy_packet;
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
        read_size = fread(&energy_packet, sizeof(ENERGY_METER_PACKET), 1, stream);
      }
    }

    if (read_size)
    {
      //remove extra new line
      energy_packet.timestamp[strlen(energy_packet.timestamp) - 1] = 0;
      convert_to_json(json_buffer, &energy_packet, json_buffer_size);

      if (!get_file_mode())
      {
        if (dummy_writes)
        {
          printf(json_buffer);
        }
        else
        {
          curl_easy_setopt(curl_handle, CURLOPT_COPYPOSTFIELDS, json_buffer);
          curl_result = curl_easy_perform(curl_handle);
          if (curl_result != CURLE_OK)
          {
            switch_to_file_mode = 1;
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(curl_result));
          }
        }
      }

      if (get_file_mode() || switch_to_file_mode)
      {
        if (   fwrite(json_buffer, strlen(json_buffer), 1, buffer_file_w) != 1
            || fwrite(JSON_STOP_TOKEN, strlen(JSON_STOP_TOKEN), 1, buffer_file_w) != 1
            || fwrite(JSON_NOTSENT_TOKEN, strlen(JSON_NOTSENT_TOKEN), 1, buffer_file_w) != 1)
        {
          fprintf(stderr, "Failed to write to buffer file\n");
          exit(EXIT_FAILURE);
        }
        fflush(buffer_file_w);
      }
    }

    //spawn a thread that tries to send the backup file contents
    if (switch_to_file_mode)
    {
      switch_to_file_mode = 0;
      set_file_mode(1);
      g_th_args.curl_handle = &curl_handle;
      g_th_args.dummy_writes = dummy_writes;
      g_th_args.file = &buffer_file_r;
      pthread_create(&g_thread, NULL, backup_sender_thread, &g_th_args);
    }

    usleep(1000 * JSON_SENDER_LOOP_CLOCK);
  }



  //cleanup curl
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();

  //free json buffer
  free(json_buffer);

  //close files
  fclose(buffer_file_r);
  fclose(buffer_file_w);

  return 0;
}

