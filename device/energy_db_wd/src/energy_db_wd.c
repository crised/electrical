/*
 * energy_db_wd.c
 *
 *  Created on: 21 Sep 2013
 *      Author: rnicolau
 */

#include "energy_db_wd.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libpq-fe.h>
#include <modbus-rtu.h>
#include <errno.h>


static void exit_nicely(PGconn *conn)
{
  PQfinish(conn);
  exit(1);
}

int read_unsigned_32b(modbus_t* ctx, int addr, uint32_t* value)
{
  uint16_t lo, hi;
  int rcl, rch;
  rcl = modbus_read_registers(ctx, addr, 1, &lo);
  usleep(17000); //max transaction timing.
  rch = modbus_read_registers(ctx, addr+1, 1, &hi);
  usleep(17000); //max transaction timing.

  if ( rcl != 1 || rch != 1)
  {
    fprintf(stderr, "modbus_read_registers failed: %s\n", modbus_strerror(errno));
    return 0;
  }
  else
  {
    *value = hi * 65536 + lo;
    printf("modbus read from %d: %u,%u = %u\n", addr, lo, hi, *value);
    return 1;
  }
}

int read_signed_32b(modbus_t* ctx, int addr, int32_t* value)
{
  uint16_t lo, hi;
  int rcl, rch;
  rcl = modbus_read_registers(ctx, addr, 1, &lo);
  usleep(17000); //max transaction timing.
  rch = modbus_read_registers(ctx, addr+1, 1, &hi);
  usleep(17000); //max transaction timing.

  if ( rcl != 1 || rch != 1)
  {
    fprintf(stderr, "modbus_read_registers failed: %s\n", modbus_strerror(errno));
    return 0;
  }
  else
  {
    *value = ((int16_t)hi) * 65536 + lo;
    printf("modbus read from %d: %u,%u = %d\n", addr, lo, hi, *value);
    return 1;
  }
}

int read_from_Modbus(void* rec, const char* port, int type)
{
  int modbus_result = 1;
  modbus_t *ctx;

  ctx = modbus_new_rtu(port, 9600, 'N', 8,1);
  modbus_set_slave(ctx, 1);
  if (ctx == NULL) {
    fprintf(stderr, "Unable to create the libmodbus context\n");
    return 0;
  }

  modbus_result = (modbus_connect(ctx) == 0);
  if (!modbus_result)
  {
    printf("modbus_connect failed\n");
    modbus_result = 0;
  }

  if (modbus_result)
  {
    struct timeval response_timeout;
    response_timeout.tv_sec = 5;
    response_timeout.tv_usec = 0;
    modbus_set_response_timeout(ctx, &response_timeout);

    switch (type)
    {
      case E_INSTANT_VALUES_RECORD:
      {
        INSTANT_VALUES_RECORD* record = (INSTANT_VALUES_RECORD*)rec;
        modbus_result &= read_signed_32b(ctx, 13952,        &record->v1_voltage);
        modbus_result &= read_signed_32b(ctx, 13952 + 2,    &record->v2_voltage);
        modbus_result &= read_signed_32b(ctx, 13952 + 4,    &record->v3_voltage);
        modbus_result &= read_signed_32b(ctx, 14336,        &record->total_kw);
        modbus_result &= read_signed_32b(ctx, 14336 + 2,    &record->total_pf);
      } break;
      case E_DEMAND_VALUES_RECORD:
      {
        DEMAND_VALUES_RECORD* record = (DEMAND_VALUES_RECORD*)rec;
        modbus_result &= read_unsigned_32b(ctx, 14592 + 12, &record->kw_import_block_demand);
        modbus_result &= read_unsigned_32b(ctx, 14592 + 14, &record->kvar_import_block_demand);
        modbus_result &= read_unsigned_32b(ctx, 14592 + 16, &record->kva_block_demand);
      } break;
      case E_ENERGY_VALUES_RECORD:
      {
        ENERGY_VALUES_RECORD* record = (ENERGY_VALUES_RECORD*)rec;
        modbus_result &= read_unsigned_32b(ctx, 14720,      &record->kwh_import);
        modbus_result &= read_unsigned_32b(ctx, 14848,      &record->kwh_import_l1);
        modbus_result &= read_unsigned_32b(ctx, 14848 + 2,  &record->kwh_import_l2);
        modbus_result &= read_unsigned_32b(ctx, 14848 + 4,  &record->kwh_import_l3);
      } break;
      default:
        printf("Invalid reading type\n");
        modbus_result = 0;
      break;
    }
  }

  if (!modbus_result)
  {
    fprintf(stderr, "Modbus read failed\n");
  }


  modbus_flush(ctx);
  modbus_close(ctx);
  modbus_free(ctx);

  return modbus_result;
}

int write_to_DB(void* rec, PGconn* connection, int type)
{
  int query_result = 1;
  char command[1024];

  switch (type)
  {
    case E_INSTANT_VALUES_RECORD:
    {
      INSTANT_VALUES_RECORD* record = (INSTANT_VALUES_RECORD*)rec;
      snprintf(command, sizeof(command),
          "INSERT INTO public.instant_values_readings VALUES ("
          "DEFAULT, DEFAULT, "
          "\'%d\', \'%d\', \'%d\', \'%d\', \'%d\'"
          ")",
              record->v1_voltage,
              record->v2_voltage,
              record->v3_voltage,
              record->total_kw,
              record->total_pf
      );
    } break;
    case E_DEMAND_VALUES_RECORD:
    {
      DEMAND_VALUES_RECORD* record = (DEMAND_VALUES_RECORD*)rec;
      snprintf(command, sizeof(command),
          "INSERT INTO public.demand_values_readings VALUES ("
          "DEFAULT, DEFAULT, "
          "\'%u\', \'%u\', \'%u\'"
          ")",
              record->kw_import_block_demand,
              record->kvar_import_block_demand,
              record->kva_block_demand
      );
    } break;
    case E_ENERGY_VALUES_RECORD:
    {
      ENERGY_VALUES_RECORD* record = (ENERGY_VALUES_RECORD*)rec;
      snprintf(command, sizeof(command),
          "INSERT INTO public.energy_values_readings VALUES ("
          "DEFAULT, DEFAULT, "
          "\'%u\', \'%u\', \'%u\', \'%u\'"
          ")",
              record->kwh_import,
              record->kwh_import_l1,
              record->kwh_import_l2,
              record->kwh_import_l3
      );
    } break;
    default:
      printf("Invalid reading type\n");
      query_result = 0;
    break;
  }



  printf("%s\n", command);

  if (PQgetResult(connection) != NULL)
  {
    fprintf(stderr, "Previous query failed: %s\n", PQerrorMessage(connection));
    query_result = 0;
  }
  else
  {
    query_result = PQsendQuery(connection, command);
    if (query_result != 1)
    {
      fprintf(stderr, "Insert failed: %s\n", PQerrorMessage(connection));
    }
    else if (PQresultStatus(PQgetResult(connection)) != PGRES_COMMAND_OK)
    {
      fprintf(stderr, "Query failed: %s\n", PQerrorMessage(connection));
      query_result = 0;
    }

  }
  return query_result;
}


int main(int argc, char** argv)
{

  const char* psql_conninfo = "dbname = energyMeterDB";
  PGconn*     psql_connection;
  unsigned long long counter = 0;

  if (argc < 2)
  {
    fprintf(stderr, "usage: %s serial_port\n", argv[0]);
    exit(1);
  }

  psql_connection = PQconnectdb(psql_conninfo);
  if (PQstatus(psql_connection) != CONNECTION_OK)
  {
    fprintf(stderr, "Connection to database failed: %s\n",
        PQerrorMessage(psql_connection));
    exit_nicely(psql_connection);
  }


  while (1)
  {

    if (counter % INSTANT_VALUES_INTERVAL == 0)
    {
      INSTANT_VALUES_RECORD record;
      if (!read_from_Modbus(&record, argv[1], E_INSTANT_VALUES_RECORD))
      {
        fprintf(stderr, "read_from_Modbus failed\n");
        break;
      }
      if (!write_to_DB(&record, psql_connection, E_INSTANT_VALUES_RECORD))
      {
        fprintf(stderr, "write_to_DB failed\n");
        break;
      }
    }
    if (counter % DEMAND_VALUES_INTERVAL == 0)
    {
      DEMAND_VALUES_RECORD record;
      if (!read_from_Modbus(&record, argv[1], E_DEMAND_VALUES_RECORD))
      {
        fprintf(stderr, "read_from_Modbus failed\n");
        break;
      }
      if (!write_to_DB(&record, psql_connection, E_DEMAND_VALUES_RECORD))
      {
        fprintf(stderr, "write_to_DB failed\n");
        break;
      }
    }
    if (counter % ENERGY_VALUES_INTERVAL == 0)
    {
      ENERGY_VALUES_RECORD record;
      if (!read_from_Modbus(&record, argv[1], E_ENERGY_VALUES_RECORD))
      {
        fprintf(stderr, "read_from_Modbus failed\n");
        break;
      }
      if (!write_to_DB(&record, psql_connection, E_ENERGY_VALUES_RECORD))
      {
        fprintf(stderr, "write_to_DB failed\n");
        break;
      }
    }

    sleep(1);
    counter++;
  }

  PQfinish(psql_connection);

  return 0;
}

