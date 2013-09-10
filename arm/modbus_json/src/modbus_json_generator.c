/*
 * modbus_json_generator.c
 *
 *  Created on: 10 Sep 2013
 *      Author: rnicolau
 */

#include <unistd.h>
#include "modbus_json.h"


volatile int modbus_json_generator_loop_on = 1;

int modbus_json_generator_loop(FILE *stream)
{
  int run_number = 0;
  while (modbus_json_generator_loop_on)
  {
    //fprintf(stream, "%s\n", JSON_START_TOKEN);
    fprintf(stream, "{\n");
    fprintf(stream, "\"Run number\":\"%d\"\n", ++run_number);
    fprintf(stream, "\"Value 1\":\"1\"\n");
    fprintf(stream, "\"Value 2\":\"2\"\n");
    fprintf(stream, "\"Value 3\":\"3\"\n");
    fprintf(stream, "}\n");
    fprintf(stream, "%s\n", JSON_STOP_TOKEN);
    fflush(stream);

    sleep(3);
  }

  return 0;
}
