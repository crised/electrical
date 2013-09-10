/*
 * modbus_json_sender.c
 *
 *  Created on: 10 Sep 2013
 *      Author: rnicolau
 */

#include <string.h>
#include <malloc.h>
#include <errno.h>

#include "curl/curl.h"
#include "modbus_json.h"

volatile int modbus_json_sender_loop_on = 1;

int modbus_json_sender_loop(FILE *stream, char* URL, int dummy_writes)
{
  //buffers and helpers
  size_t json_buffer_size = 2 * 1024;
  char *json_buffer = malloc(json_buffer_size);

  //curl data
  CURL* curl_handle;
  CURLcode curl_result = CURLE_OK;

  //Initialise curl
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();
  if (curl_handle == NULL)
  {
    modbus_json_sender_loop_on = 0;
  }
  else
  {
    curl_easy_setopt(curl_handle, CURLOPT_URL, URL);
  }

  while (modbus_json_sender_loop_on)
  {
    ENERGY_METER_PACKET energy_packet;
    int count;
    size_t read_size = fread(&energy_packet, sizeof(ENERGY_METER_PACKET), 1, stream);

    if (read_size)
    {
      //remove extra new line
      energy_packet.timestamp[strlen(energy_packet.timestamp) - 1] = 0;

      if (energy_packet.type == ENERGY_METER_PACK_TYPE_ENERGY)
      {

        count = snprintf(json_buffer, json_buffer_size,
            "{\n"
            "\"TimeStamp\":\"%s\",\n"
            "\"id\":%lu,\n"
            "\"energy\":%u\n"
            "}\n",
            energy_packet.timestamp,
            energy_packet.id,
            energy_packet.payload.tab_reg[0]
            );
      }
      else if (energy_packet.type == ENERGY_METER_PACK_TYPE_DETAILS)
      {

        count = snprintf(json_buffer, json_buffer_size,
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
            energy_packet.timestamp,
            energy_packet.id,
            energy_packet.payload.tab_reg[VOLTAGE_1],
            energy_packet.payload.tab_reg[VOLTAGE_2],
            energy_packet.payload.tab_reg[VOLTAGE_3],
            energy_packet.payload.tab_reg[VOLTAGE_12],
            energy_packet.payload.tab_reg[VOLTAGE_23],
            energy_packet.payload.tab_reg[VOLTAGE_31],
            energy_packet.payload.tab_reg[CURRENT_1],
            energy_packet.payload.tab_reg[CURRENT_2],
            energy_packet.payload.tab_reg[CURRENT_3],
            energy_packet.payload.tab_reg[ACT_POWER_1],
            energy_packet.payload.tab_reg[ACT_POWER_2],
            energy_packet.payload.tab_reg[ACT_POWER_3],
            energy_packet.payload.tab_reg[REACT_POWER_1],
            energy_packet.payload.tab_reg[REACT_POWER_2],
            energy_packet.payload.tab_reg[REACT_POWER_3],
            energy_packet.payload.tab_reg[CURRENT_N],
            energy_packet.payload.tab_reg[FREQUENCY],
            energy_packet.payload.tab_reg[APP_POWER_1],
            energy_packet.payload.tab_reg[APP_POWER_2],
            energy_packet.payload.tab_reg[APP_POWER_3]
            );
      }


      if (count < 0)
      {
        fprintf(stderr, "Failed to format json data using snprintf: %s\n",
            strerror(errno));
        exit(EXIT_FAILURE);
      }
      if (json_buffer_size <= count)
      {
        fprintf(stderr,
            "Buffer is not big enough (%u) to format json data. %s\n",
            (int) json_buffer_size, strerror(errno));
        exit(EXIT_FAILURE);
      }

      if (dummy_writes)
      {
        printf(json_buffer);
      }
      else
      {
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, json_buffer);
        curl_result = curl_easy_perform(curl_handle);
        if (curl_result != CURLE_OK)
        {
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(curl_result));
        }
      }
    }
  }



  //cleanup curl
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();

  //free json buffer
  free(json_buffer);

  return 0;
}

