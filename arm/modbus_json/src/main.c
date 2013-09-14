/*
 * main.c
 *
 *  Created on: 9 Sep 2013
 *      Author: rnicolau
 */

#include <unistd.h>

#include "modbus_json.h"

#ifndef USE_DUMMY_MODBUS
#define USE_DUMMY_MODBUS (0)
#endif

#ifndef USE_DUMMY_HTTP
#define USE_DUMMY_HTTP (0)
#endif

int main(int argc, char** argv)
{

  int file_descriptors[2];
  pid_t process_id;

  if (argc < 3)
  {
    printf("Usage:\n"
        " %s json-destination-url modbus-device-ip modebus-device-port\n"
        " or\n"
        " %s json-destination-url serial-device\n", argv[0], argv[0]);
    exit(EXIT_FAILURE);
  }

  openlog(NULL, LOG_CONS | LOG_PID, LOG_USER);

  //create pipes
  pipe(file_descriptors);

  //create child process
  process_id = fork();

  //check fork() result
  switch(process_id)
  {
  case -1:
    CRIT("Failed to start process\n");
    exit(EXIT_FAILURE);
    break;
  case 0:
    {
      //child process, use read end
      FILE *stream;
      close(file_descriptors[1]);
      stream = fdopen(file_descriptors[0], "r");
      modbus_json_sender_loop(stream, argv[1], USE_DUMMY_HTTP);
      close(file_descriptors[0]);
    }
    break;
  default:
    {
      //parent process, use write end
      FILE *stream;
      close(file_descriptors[0]);
      stream = fdopen(file_descriptors[1], "w");
      modbus_serial_main(argc, argv, stream, USE_DUMMY_MODBUS);
      close(file_descriptors[1]);
    }
    break;
  }

  return 0;
}
