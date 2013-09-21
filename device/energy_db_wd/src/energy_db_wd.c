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
#include <modbus/modbus-rtu.h>
#include <errno.h>


static void exit_nicely(PGconn *conn)
{
  PQfinish(conn);
  exit(1);
}

int read_unsigned_32b(modbus_t* ctx, int addr, uint32_t* value)
{
  uint16_t lo, hi;
  if (   modbus_read_registers(ctx, addr, 1, &lo) != 1
      || modbus_read_registers(ctx, addr+1, 1, &hi) != 1)
  {
    fprintf(stderr, "modbus_read_registers failed: %d", errno);
    return 0;
  }
  else
  {
    *value = hi * 65536 + lo;
    printf("modbus read from %d: %u,%u = %u", addr, lo, hi, *value);
    return 1;
  }
}

int read_signed_32b(modbus_t* ctx, int addr, int32_t* value)
{
  uint16_t lo, hi;
  if (   modbus_read_registers(ctx, addr, 1, &lo) != 1
      || modbus_read_registers(ctx, addr+1, 1, &hi) != 1)
  {
    fprintf(stderr, "modbus_read_registers failed: %d", errno);
    return 0;
  }
  else
  {
    *value = ((int16_t)hi) * 65536 + lo;
    printf("modbus read from %d: %u,%u = %d", addr, lo, hi, *value);
    return 1;
  }
}

int read_from_Modbus(ENERGY_RECORD* record, const char* port)
{
  int modbus_result = 1;
  modbus_t *ctx;

  ctx = modbus_new_rtu(port, 9600, 'N', 8,1);
  modbus_set_slave(ctx, 1);
  if (ctx == NULL) {
    fprintf(stderr, "Unable to create the libmodbus context\n");
    return 0;
  }

  modbus_result = modbus_connect(ctx);
  if (modbus_result)
  {
    printf("modbus_connect failed\n");
    modbus_result = 0;
  }

  if (modbus_result)
  {
    modbus_result &= read_signed_32b(ctx, 13952,        &record->v1_voltage);
    modbus_result &= read_signed_32b(ctx, 13952 + 2,    &record->v2_voltage);
    modbus_result &= read_signed_32b(ctx, 13952 + 4,    &record->v3_voltage);

    modbus_result &= read_signed_32b(ctx, 14336,        &record->total_kw);
    modbus_result &= read_signed_32b(ctx, 14336 + 2,    &record->total_pf);

    modbus_result &= read_unsigned_32b(ctx, 14592 + 12, &record->kw_import_block_demand);
    modbus_result &= read_unsigned_32b(ctx, 14592 + 14, &record->kvar_import_block_demand);
    modbus_result &= read_unsigned_32b(ctx, 14592 + 16, &record->kva_block_demand);

    modbus_result &= read_unsigned_32b(ctx, 14720,      &record->kwh_import);

    modbus_result &= read_unsigned_32b(ctx, 14848,      &record->kwh_import_l1);
    modbus_result &= read_unsigned_32b(ctx, 14848 + 2,  &record->kwh_import_l2);
    modbus_result &= read_unsigned_32b(ctx, 14848 + 4,  &record->kwh_import_l3);
  }

  if (!modbus_result)
  {
    fprintf(stderr, "Modbus read failed");
  }


  modbus_flush(ctx);
  modbus_close(ctx);
  modbus_free(ctx);

  return modbus_result;
}

int write_to_DB(ENERGY_RECORD* record, PGconn* connection)
{
  int query_result = 1;
  char command[1024];

  snprintf(command, sizeof(command),
      "INSERT INTO public.energyreadings VALUES ("
      "DEFAULT, \'%s\', "
      "\'%d\', \'%d\', \'%d\', \'%d\', \'%d\', "
      "\'%u\', \'%u\', \'%u\', \'%u\', \'%u\', \'%u\', \'%u\'"
      ")",

      record->sent? "true" : "false",
          record->v1_voltage,
          record->v2_voltage,
          record->v3_voltage,
          record->total_kw,
          record->total_pf,
          record->kw_import_block_demand,
          record->kvar_import_block_demand,
          record->kva_block_demand,
          record->kwh_import,
          record->kwh_import_l1,
          record->kwh_import_l2,
          record->kwh_import_l3
  );

  printf("%s\n", command);

  if (PQgetResult(connection) != NULL)
  {
    fprintf(stderr, "Previous query failed: %s", PQerrorMessage(connection));
    query_result = 0;
  }
  else
  {
    query_result = PQsendQuery(connection, command);
    if (query_result != 1)
    {
      fprintf(stderr, "Insert failed: %s", PQerrorMessage(connection));
    }
    else if (PQresultStatus(PQgetResult(connection)) != PGRES_COMMAND_OK)
    {
      fprintf(stderr, "Query failed: %s", PQerrorMessage(connection));
      query_result = 0;
    }

  }
  return query_result;
}


int main(int argc, char** argv)
{

  const char* psql_conninfo = "dbname = energyMeterDB";
  PGconn*     psql_connection;

  psql_connection = PQconnectdb(psql_conninfo);
  if (PQstatus(psql_connection) != CONNECTION_OK)
  {
    fprintf(stderr, "Connection to database failed: %s",
        PQerrorMessage(psql_connection));
    exit_nicely(psql_connection);
  }


  while (1)
  {
    ENERGY_RECORD rec = {0};

    if (!read_from_Modbus(&rec, "/dev/ttyUSB0"))
    {
      fprintf(stderr, "read_from_Modbus failed");
      break;
    }
    if (!write_to_DB(&rec, psql_connection))
    {
      fprintf(stderr, "write_to_DB failed");
      break;
    }
    sleep(3);
  }

  PQfinish(psql_connection);

  return 0;
}

