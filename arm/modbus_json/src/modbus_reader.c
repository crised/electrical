/*
 * Author: Peter Csaszar - 10 Sept 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <modbus-rtu.h>

#include "modbus_json.h"






void readAndSendEnergy(modbus_t * ctx, long unsigned counter, FILE *stream, int dummy_readings)
{
  ENERGY_METER_PACKET energy_packet;
  time_t t = time(NULL);

  energy_packet.type = ENERGY_METER_PACK_TYPE_ENERGY;
  sprintf(energy_packet.timestamp, "%s", asctime(gmtime(&t)));
  energy_packet.id = counter;

  if (dummy_readings == 0) {
    int rc = modbus_read_registers(ctx, ACT_TOTAL_ENERGY, 1, energy_packet.payload.tab_reg);
    if (rc == -1) {
      fprintf(stderr, "Failed to read energy register. %s\n", modbus_strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  fwrite(&energy_packet, sizeof(ENERGY_METER_PACKET), 1, stream);
  fflush(stream);
}

void readAndSendDetails(modbus_t * ctx, long unsigned counter, FILE *stream, int dummy_readings)
{
  ENERGY_METER_PACKET energy_packet;
  time_t t = time(NULL);

  energy_packet.type = ENERGY_METER_PACK_TYPE_DETAILS;
  sprintf(energy_packet.timestamp, "%s", asctime(gmtime(&t)));
  energy_packet.id = counter;

  if (dummy_readings == 0) {
    int rc = modbus_read_registers(ctx, REGISTERS_FROM, NUM_OF_REGISTERS, energy_packet.payload.tab_reg);
    if (rc == -1) {
      fprintf(stderr, "Failed to read registers. %s\n", modbus_strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  fwrite(&energy_packet, sizeof(ENERGY_METER_PACKET), 1, stream);
  fflush(stream);
}

modbus_t * init_modbus_serial(const char * serial_device)
{
  modbus_t * ctx;

  ctx = modbus_new_rtu(serial_device, 115200, 'N', 8, 1);
  if(ctx == NULL){
    fprintf(stderr, "Unable to create the libmodbus context\n");
    exit(EXIT_FAILURE);
  }

  if(modbus_connect(ctx) == -1){
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    exit(EXIT_FAILURE);
  }

  if(modbus_set_slave(ctx, 1) == -1){
    fprintf(stderr, "Setting slave failed: %s\n", modbus_strerror(errno));
    modbus_close(ctx);
    modbus_free(ctx);
    exit(EXIT_FAILURE);
  }

  return ctx;
}

modbus_t * init_modbus_tcp(const char * ip, const char * port)
{
  modbus_t * ctx;

  ctx = modbus_new_tcp_pi(ip, port);
  if(ctx == NULL){
    fprintf(stderr, "Unable to allocate libmodbus context\n");
    exit(EXIT_FAILURE);
  }

  if(modbus_connect(ctx) == -1){
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    exit(EXIT_FAILURE);
  }

  return ctx;
}

void modbus_reader(modbus_t * ctx, FILE *stream, int dummy_readings)
{
  long unsigned counter = 0;

  while(1){
    if(counter % (ENERGY_READ_PERIOD + 1))
      readAndSendDetails(ctx, counter++, stream, dummy_readings);
    readAndSendEnergy(ctx, counter++, stream, dummy_readings);

    usleep(DETAIL_READ_PERIOD);
  }

  modbus_close(ctx);
  modbus_free(ctx);
}



#ifdef DEBUG
#define EMERG(...)      fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define ALERT(...)      fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define CRIT(...)       fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define ERR(...)        fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define WARNING(...)    fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define NOTICE(...)     fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define INFO(...)       fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define DBG(...)        fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#else
#define EMERG(...)      syslog(LOG_EMERG, __VA_ARGS__)
#define ALERT(...)      syslog(LOG_ALERT, __VA_ARGS__)
#define CRIT(...)       syslog(LOG_CRIT, __VA_ARGS__)
#define ERR(...)        syslog(LOG_ERR, __VA_ARGS__)
#define WARNING(...)    syslog(LOG_WARNING, __VA_ARGS__)
#define NOTICE(...)     syslog(LOG_NOTICE, __VA_ARGS__)
#define INFO(...)       syslog(LOG_INFO, __VA_ARGS__)
#define DBG(...)        syslog(LOG_DEBUG, __VA_ARGS__)
#endif


static void daemonize(const char * name)
{
  openlog(name, LOG_CONS | LOG_PID, LOG_DAEMON);

  INFO("Daemonizing: uid: %d, euid: %d, gid: %d, egid: %d",
      getuid(), geteuid(), getgid(), getegid());

  pid_t pid, sid;

  pid = fork();
  if (pid < 0) {
    CRIT("Fork to daemonize failed. Error: %m");
  }

  if (0 < pid) {
    INFO("We have the daemon child with pid %d. Exiting from parent process ...", pid);
    printf("Daemonized.\n");
    exit(EXIT_SUCCESS);
  }

  INFO("daemonized");

  signal(SIGHUP, SIG_IGN); /* not to exit when terminal is closed */

  umask(0); /* Change the file mode mask */

  sid = setsid();
  if (sid < 0) {
    ERR("Failed to create new SID for child process. Error: %m");
    exit(EXIT_FAILURE);
  }

  /* Redirect standard files to /dev/null */
  if(!freopen( "/dev/null", "r", stdin))
    WARNING("Failed to reopen stdin");
  if(!freopen( "/dev/null", "w", stdout))
    WARNING("Failed to reopen stdout");
  if(!freopen( "/dev/null", "w", stderr))
    WARNING("Failed to reopen stderr");
}


int modbus_serial_main(int argc, char *args[], FILE *stream, int dummy_readings)
{
//  if(argc < 3){
//    printf( "Usage:\n"
//      " %s json-destination-url modbus-device-ip modebus-device-port\n"
//      " or\n"
//      " %s json-destination-url serial-device\n",
//      args[0], args[0]);
//    exit(EXIT_FAILURE);
//  }
//
//#ifndef DEBUG
//  daemonize(args[0]);
//#endif
//
//  int pipefd[2];
//
//  if(pipe(pipefd) == -1){
//    ERR("Failed to create pipe. %m");
//    exit(EXIT_FAILURE);
//  }
//
//  int pid = fork();
//  if (pid < 0) {
//    CRIT("Fork json sender failed. Error: %m");
//  }
//
//  if (0 < pid) { // parent process should read serial data and send json by pipe
//    close(pipefd[0]); // Close unused read end


    modbus_t * ctx = NULL;

    if(dummy_readings == 0)
    {
      if(argc == 3)
        ctx = init_modbus_serial(args[2]);
      else
        ctx = init_modbus_tcp(args[2], args[3]);
    }

    modbus_reader(ctx, stream, dummy_readings);

//    wait(NULL); // wait for child process (never gets control however)
    return 0;
//  } else { // pid == 0
//    close(pipefd[1]); // close unused write end
//    printf("Child to send json data...");
//  }
//
//  closelog();
  return 0;
}
