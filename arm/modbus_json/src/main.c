/*
 * main.c
 *
 *  Created on: 9 Sep 2013
 *      Author: rnicolau
 */

#include <unistd.h>
#include <getopt.h>

#include "modbus_json.h"

#ifndef USE_DUMMY_MODBUS
#define USE_DUMMY_MODBUS (0)
#endif

#ifndef USE_DUMMY_HTTP
#define USE_DUMMY_HTTP (0)
#endif

static const struct option long_options[] =
{
  { "energy_url",     1, NULL, 'n' },
  { "error_url",      1, NULL, 'e' },
  { "serial",         1, NULL, 's' },
  { "ip",             1, NULL, 'i' },
  { "port",           1, NULL, 'p' },
  { NULL, 0, NULL, 0 }
};

static const char* short_options = "n:e:s:i:p:";

void print_usage()
{
  printf("Usage:\n"
      "-n --energy_url [energy reporting URL]\n"
      "-e --error_url [error reporting URL]\n"
      "-s --serial [Modbus serial port]\n"
      "-i --ip [Modbus IP]\n"
      "-p --port [Modbus IP port]\n");
}

int main(int argc, char** argv)
{

  int file_descriptors[2];
  pid_t process_id;
  char* energy_url = NULL;
  char* error_url = NULL;
  char* serial = NULL;
  char* ip = NULL;
  char* port = NULL;
  int next_option;

  do
  {
    next_option = getopt_long (argc, argv, short_options, long_options, NULL);
    switch (next_option)
    {
    case 'n': energy_url = optarg; break;
    case 'e': error_url = optarg; break;
    case 's': serial = optarg; break;
    case 'i': ip = optarg; break;
    case 'p': port = optarg; break;
    }
  } while (next_option != -1);

  if ((energy_url == NULL || error_url == NULL)
      || (serial == NULL && (ip == NULL || port == NULL)))
  {
    print_usage();
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
      modbus_json_sender_loop(stream, energy_url, error_url, USE_DUMMY_HTTP);
      close(file_descriptors[0]);
    }
    break;
  default:
    {
      //parent process, use write end
      FILE *stream;
      close(file_descriptors[0]);
      stream = fdopen(file_descriptors[1], "w");
      modbus_serial_main(serial, ip, port, stream, USE_DUMMY_MODBUS);
      close(file_descriptors[1]);
    }
    break;
  }

  return 0;
}
