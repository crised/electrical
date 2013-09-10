/*
 * modbus_json.h
 *
 *  Created on: 10 Sep 2013
 *      Author: rnicolau
 */

//#define JSON_START_TOKEN  "#START\n"
#define JSON_STOP_TOKEN   "#STOP\n"

#include <stdio.h>

int modbus_json_generator_loop(FILE *stream);
int modbus_json_sender_loop(FILE *stream, char* URL);


