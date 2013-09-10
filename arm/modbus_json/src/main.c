/*
 * main.c
 *
 *  Created on: 9 Sep 2013
 *      Author: rnicolau
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "modbus_json.h"

int main(int argc, char** argv)
{

  int file_descriptors[2];
  pid_t process_id;

  if (argc < 2)
  {
    fprintf(stderr, "usage: modbus_json URL\n");
    return 1;
  }

  //create pipes
  pipe(file_descriptors);

  //create child process
  process_id = fork();

  if (process_id == (pid_t)0)
  {
    //child process, use read end
    FILE *stream;
    close(file_descriptors[1]);
    stream = fdopen(file_descriptors[0], "r");
    modbus_json_sender_loop(stream, argv[1]);
    close(file_descriptors[0]);
  }
  else
  {
    //parent process, use write end
    FILE *stream;
    close(file_descriptors[0]);
    stream = fdopen(file_descriptors[1], "w");
    modbus_json_generator_loop(stream);
    close(file_descriptors[1]);
  }

  return 0;
}
