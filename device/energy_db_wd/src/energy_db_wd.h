/*
 * energy_db_wd.h
 *
 *  Created on: 21 Sep 2013
 *      Author: rnicolau
 */

#ifndef ENERGY_DB_WD_H_
#define ENERGY_DB_WD_H_

#include <stdint.h>

#define E_INSTANT_VALUES_RECORD (1)
#define E_DEMAND_VALUES_RECORD  (1<<1)
#define E_ENERGY_VALUES_RECORD  (1<<2)

#define INSTANT_VALUES_INTERVAL (1)
#define DEMAND_VALUES_INTERVAL  (15)
#define ENERGY_VALUES_INTERVAL  (5)

typedef struct INSTANT_VALUES_RECORD
{
  int32_t  v1_voltage;
  int32_t  v2_voltage;
  int32_t  v3_voltage;
  int32_t  total_kw;
  int32_t  total_pf;
}INSTANT_VALUES_RECORD;

typedef struct DEMAND_VALUES_RECORD
{
  uint32_t kw_import_block_demand;
  uint32_t kvar_import_block_demand;
  uint32_t kva_block_demand;
}DEMAND_VALUES_RECORD;

typedef struct ENERGY_VALUES_RECORD
{
  uint32_t kwh_import;
  uint32_t kwh_import_l1;
  uint32_t kwh_import_l2;
  uint32_t kwh_import_l3;
}ENERGY_VALUES_RECORD;



#endif /* ENERGY_DB_WD_H_ */
