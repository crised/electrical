/*
 * modbus_json_sender.c
 *
 *  Created on: 10 Sep 2013
 *      Author: rnicolau
 */

#include <string.h>
#include <malloc.h>
#include "curl/curl.h"

#include "modbus_json.h"

volatile int modbus_json_sender_loop_on = 1;

int modbus_json_sender_loop(FILE *stream, char* URL)
{
  //buffers and helpers
  size_t json_buffer_size = 10 * 1024;
  char *json_buffer = malloc(json_buffer_size);
  char *json_buffer_pointer = json_buffer;
  char buffer[1024];

  //curl data
  CURL* curl_handle;
  CURLcode curl_result;

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

  while (modbus_json_sender_loop_on
      && !feof(stream)
      && !ferror(stream)
      && fgets(buffer, sizeof(buffer), stream) != NULL)
  {

    //end of JSON received, post data
    if (strcmp(buffer, JSON_STOP_TOKEN) == 0)
    {
      printf(json_buffer);

      curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, json_buffer);
      curl_result = curl_easy_perform(curl_handle);
      if(curl_result != CURLE_OK)
      {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(curl_result));
      }

      json_buffer_pointer = json_buffer;
    }
    else //continue to collect data
    {
      size_t size = strlen(buffer);
      //need more memory?
      if (json_buffer_size < (json_buffer_pointer - json_buffer + size))
      {
        size_t used_size = json_buffer_pointer - json_buffer;
        json_buffer_size += 10 * 1024;
        json_buffer = realloc(json_buffer, json_buffer_size);
        json_buffer_pointer = json_buffer + used_size;
        printf("Reallocated json buffer to %u\n", (unsigned int)json_buffer_size);
      }
      //append new line into buffer
      memcpy(json_buffer_pointer, buffer, size);
      json_buffer_pointer += size;
    }
  }

  //cleanup curl
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();

  //free json buffer
  free(json_buffer);

  return 0;
}

