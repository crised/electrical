/*
 * cenergy.c
 *
 *  Created on: 21 Sep 2013
 *      Author: rnicolau
 */

#include "cenergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libpq-fe.h>
#include <modbus-rtu.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <pthread.h>



static void exit_nicely(PGconn *conn)
{
  PQfinish(conn);
  exit(EXIT_FAILURE);
}

int read_modbus_32b(modbus_t* ctx, int addr, uint32_t* value, int is_signed)
{
  uint16_t reg[2];
  int rc;
  int retries = MAX_MODBUS_RETRIES;
  while (--retries)
  {
    rc = modbus_read_registers(ctx, addr, 2, reg);
    usleep(MAX_MODBUS_TR_TIMING * 2); //max transaction timing.
    if ( rc == 2)
    {
      break;
    }
  }
  if (retries == 0)
  {
    SYSLOG("modbus_read_registers %d failed: %s\n", addr, modbus_strerror(errno));
    return 0;
  }
  else
  {
    *value = reg[1] * 65536 + reg[0];
    //printf("modbus read from %d: %u,%u = %u\n", addr, reg[0], reg[1], *value);
    return 1;
  }
}



int read_from_Modbus(modbus_t *ctx, uint32_t* values, enum E_VALUES_RECORD type)
{
  int modbus_result = 1;
  int i = 0;
  METER_REGISTER_ARRAY meter_register_array[] = METER_REGISTER_ARRAY_INITIALIZER;
  METER_REGISTER* meter_register = meter_register_array[type];

  while (meter_register[i].name != 0 && modbus_result)
  {
    modbus_result = read_modbus_32b(ctx, meter_register[i].address, values, meter_register[i].is_signed);
    i++;
    values++;
  }

  return modbus_result;
}

int write_to_DB(PGconn* connection, uint32_t* values, enum E_VALUES_RECORD type)
{
  int query_result = 1;
  int i = 0;
  char command[SQL_QUERY_STRING_MAX_SIZE];
  char* pc = command;
  char* table_names[] = TABLE_NAME_ARRAY_INITIALIZER;
  METER_REGISTER_ARRAY meter_register_array[] = METER_REGISTER_ARRAY_INITIALIZER;
  METER_REGISTER* meter_register = meter_register_array[type];

  //build query format string
  //header
  sprintf(command,
      "INSERT INTO %s VALUES (DEFAULT, DEFAULT, DEFAULT, ",
      table_names[type]
      );
  pc = &command[strlen(command) - 1];

  //values
  while (meter_register[i].name != 0)
  {
    char value_string[128];

    sprintf(value_string,
        (meter_register[i].is_signed? "\'%d\', " : "\'%u\', "),
        (meter_register[i].is_signed? (int32_t)*values : *values));
    sprintf(pc, "%s", value_string);
    pc = &command[strlen(command) - 1];
    i++;
    values++;
  }
  //replace the last comma with a bracket
  *(pc - 1) = ')';

  if (PQgetResult(connection) != NULL)
  {
    SYSLOG("Previous query failed: %s\n", PQerrorMessage(connection));
    query_result = 0;
  }
  else
  {
    query_result = PQsendQuery(connection, command);
    if (query_result != 1)
    {
      SYSLOG("Insert failed: %s\n", PQerrorMessage(connection));
      SYSLOG("%s\n", command);
    }
    else if (PQresultStatus(PQgetResult(connection)) != PGRES_COMMAND_OK)
    {
      SYSLOG("Query failed: %s\n", PQerrorMessage(connection));
      SYSLOG("%s\n", command);
      query_result = 0;
    }

  }
  return query_result;
}

PGconn*     psql_connection = NULL;
modbus_t*   ctx = NULL;
volatile unsigned int watch_counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t watchdog_thread;

void ping_watchdog()
{
  int rl = pthread_mutex_trylock(&mutex);
  if (rl)
  {
    SYSLOG("Failed to lock mutex with error %d\n", rl);
  }
  watch_counter = 0;
  pthread_mutex_unlock(&mutex);
}

void *watch_dog(void *threadarg)
{
  int rl;
  while (1)
  {
    sleep(1);
    rl = pthread_mutex_trylock(&mutex);
    if (rl)
    {
      SYSLOG("Failed to lock mutex with error %d\n", rl);
    }

    watch_counter++;

    if (watch_counter > WATCHDOG_WARNING)
    {
      SYSLOG("Warning,  watchdog value is %d\n", watch_counter);
      if (watch_counter > WATCHDOG_TIMEOUT)
      {
        SYSLOG("Watchdog error\n");
        exit_nicely(psql_connection);
      }
    }

    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void cleanup_modbus(modbus_t** pctx)
{
  modbus_flush(*pctx);
  modbus_close(*pctx);
  modbus_free(*pctx);
  *pctx = NULL;
}

int setup_modbus(const char* port, modbus_t** pctx)
{
  int result = 1;

  *pctx = modbus_new_rtu(port, SERIAL_BAUDRATE, SERIAL_PARITY, SERIAL_DATA_BITS, SERIAL_STOP_BITS);
  if (*pctx == NULL)
  {
    SYSLOG("Unable to create the libmodbus context\n");
    result = 0;
  }
  if (modbus_connect(*pctx) == -1)
  {
    SYSLOG("Modbus_connect failed on port %s\n", port);
    result = 0;
  }
  if (modbus_set_slave(*pctx, 1) == -1)
  {
    SYSLOG("Modbus_connect failed on port %s\n", port);
    result = 0;
  }

#if INCREASE_MODBUS_TIMEOUT
  {
    struct timeval response_timeout;
    response_timeout.tv_sec = INCREASE_MODBUS_TIMEOUT;
    response_timeout.tv_usec = 0;
    modbus_set_response_timeout(*pctx, &response_timeout);
  }
#endif

  if (!result)
  {
    cleanup_modbus(pctx);
  }

  return result;
}


static void signal_handler(int signal)
{
  if (psql_connection)
  {
    PQfinish(psql_connection);
  }
  if (ctx)
  {
    cleanup_modbus(&ctx);
  }
  exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{

  unsigned long long counter = 0;
  int result = EXIT_SUCCESS;
  openlog(argv[0], LOG_CONS | LOG_PID, LOG_LOCAL1);
  setlogmask(LOG_UPTO(LOG_NOTICE));

  if (argc < 2)
  {
    SYSLOG("usage: %s serial_port\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (signal(SIGINT, signal_handler) == SIG_ERR
    ||signal(SIGTERM, signal_handler) == SIG_ERR
    ||signal(SIGQUIT, signal_handler) == SIG_ERR)
  {
    SYSLOG("An error occurred while setting a signal handler.\n");
    return EXIT_FAILURE;
  }

  if (!setup_modbus(argv[1], &ctx))
  {
    return EXIT_FAILURE;
  }

  psql_connection = PQconnectdb(PSQL_CONNECTION_STR);
  if (PQstatus(psql_connection) != CONNECTION_OK)
  {
    SYSLOG("Connection to database failed: %s\n",
        PQerrorMessage(psql_connection));
    exit_nicely(psql_connection);
  }

  //start watchdog thread
  {
    int rc = pthread_create(&watchdog_thread, NULL, watch_dog, NULL);
    if (rc)
    {
      SYSLOG("Failed to create thread with error %d\n", rc);
      exit_nicely(psql_connection);
    }
  }

  while (1)
  {
    uint32_t values[METER_REGISTER_MAX_SIZE];
    int modbus_failed = 0;
    int psql_failed = 0;

    if (counter % INSTANT_VALUES_INTERVAL == 0)
    {
      if (!read_from_Modbus(ctx, values, E_INSTANT_VALUES_RECORD))
      {
        SYSLOG("read_from_Modbus failed\n");
        modbus_failed = 1;
      }
      if (!modbus_failed && !write_to_DB(psql_connection, values, E_INSTANT_VALUES_RECORD))
      {
        SYSLOG("write_to_DB failed\n");
        psql_failed = 1;
      }
    }
    if (counter % DEMAND_VALUES_INTERVAL == 0)
    {
      if (!read_from_Modbus(ctx, values, E_DEMAND_VALUES_RECORD))
      {
        SYSLOG("read_from_Modbus failed\n");
        modbus_failed = 1;
      }
      if (!modbus_failed && !write_to_DB(psql_connection, values, E_DEMAND_VALUES_RECORD))
      {
        SYSLOG("write_to_DB failed\n");
        psql_failed = 1;
      }
    }
    if (counter % ENERGY_VALUES_INTERVAL == 0)
    {
      if (!read_from_Modbus(ctx, values, E_ENERGY_VALUES_RECORD))
      {
        SYSLOG("read_from_Modbus failed\n");
        modbus_failed = 1;
      }
      if (!modbus_failed && !write_to_DB(psql_connection, values, E_ENERGY_VALUES_RECORD))
      {
        SYSLOG("write_to_DB failed\n");
        psql_failed = 1;
      }
    }

    if (psql_failed)
    {
      result = EXIT_FAILURE;
      break;
    }

    //retry modbus
    if (modbus_failed)
    {
      cleanup_modbus(&ctx);
      if (setup_modbus(argv[1], &ctx))
      {
        SYSLOG("Modbus reconnection succeeded\n");
        modbus_failed = 0;
      }
      else
      {
        //retry until success
        while (1)
        {
          uint32_t wait_counter = MODBUS_ERROR_WAIT_TIME;
          SYSLOG("Modbus reconnection failed, retrying...\n");
          //wait some
          while (wait_counter--)
          {
            usleep(500*1000);
            ping_watchdog();
            usleep(500*1000);
          }
          if (setup_modbus(argv[1], &ctx))
          {
            SYSLOG("Modbus reconnection succeeded\n");
            break;
          }
        }
      }
    }


    //sleep until next full second;
    {
      struct timespec tp;
      clock_gettime(CLOCK_REALTIME, &tp);
      usleep((1000000000 - tp.tv_nsec % 1000000000) / 1000);
    }
    counter++;
    ping_watchdog();
  }

  PQfinish(psql_connection);
  cleanup_modbus(&ctx);

  return result;
}
