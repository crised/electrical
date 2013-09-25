/*
 * energy_db_wd.h
 *
 *  Created on: 21 Sep 2013
 *      Author: rnicolau
 */

#ifndef ENERGY_DB_WD_H_
#define ENERGY_DB_WD_H_

#include <stdint.h>

enum E_VALUES_RECORD
{
  E_INSTANT_VALUES_RECORD = 0,
  E_DEMAND_VALUES_RECORD,
  E_ENERGY_VALUES_RECORD,
  E_VALUES_RECORD_COUNT
};

#define INSTANT_VALUES_INTERVAL (1)
#define DEMAND_VALUES_INTERVAL  (15)
#define ENERGY_VALUES_INTERVAL  (5)

#define MAX_MODBUS_TR_TIMING    (17000)
#define MAX_MODBUS_RETRIES      (5)
#define MAX_MODBUS_READ_SIZE    (10)
#define INCREASE_MODBUS_TIMEOUT (0)

typedef struct METER_REGISTER
{
  char*   name;
  int     address;
  int     is_signed;
}METER_REGISTER;


#define INSTANT_VALUES_METER_REGISTERS \
{\
{"v1_voltage",    13952,        0}, \
{"v2_voltage",    13952 + 2,    0}, \
{"v3_voltage",    13952 + 4,    0}, \
{"total_kw",      14336 ,       1}, \
{"total_pf",      14336 + 2 ,   1}, \
{0, 0, 0}, \
}

#define DEMAND_VALUES_METER_REGISTERS \
{\
{"kw_import_block_demand",      14592 + 12,        0}, \
{"kvar_import_block_demand",    14592 + 14,        0}, \
{"kva_block_demand",            14592 + 16,        0}, \
{0, 0, 0}, \
}

#define ENERGY_VALUES_METER_REGISTERS \
{\
{"kwh_import",      14720,        0}, \
{"kwh_import_l1",   14848,        0}, \
{"kwh_import_l2",   14848 + 2,    0}, \
{"kwh_import_l3",   14848 + 4,    0}, \
{0, 0, 0}, \
}

#define METER_REGISTER_MAX_SIZE (6)
typedef METER_REGISTER METER_REGISTER_ARRAY[METER_REGISTER_MAX_SIZE];

#define METER_REGISTER_ARRAY_INITIALIZER \
{\
    INSTANT_VALUES_METER_REGISTERS, \
    DEMAND_VALUES_METER_REGISTERS,  \
    ENERGY_VALUES_METER_REGISTERS   \
}


#define TABLE_NAME_ARRAY_INITIALIZER \
{\
    "public.instant", \
    "public.demand",  \
    "public.energy"   \
}

#define SQL_QUERY_STRING_MAX_SIZE (1024)



#ifdef DEBUG
  #define SYSLOG(...)      fprintf(stderr, __VA_ARGS__);
#else
  #define SYSLOG(...)       syslog(LOG_CRIT, __VA_ARGS__)
#endif


#endif /* ENERGY_DB_WD_H_ */
