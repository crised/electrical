/*
 * energy_db_wd.h
 *
 *  Created on: 21 Sep 2013
 *      Author: rnicolau
 */

#ifndef ENERGY_DB_WD_H_
#define ENERGY_DB_WD_H_

#include <stdint.h>


typedef struct ENERGY_RECORD
{
  char     datetime[40];
  uint8_t  sent;
  int32_t  v1_voltage;
  int32_t  v2_voltage;
  int32_t  v3_voltage;
  int32_t  total_kw;
  int32_t  total_pf;
  uint32_t kw_import_block_demand;
  uint32_t kvar_import_block_demand;
  uint32_t kva_block_demand;
  uint32_t kwh_import;
  uint32_t kwh_import_l1;
  uint32_t kwh_import_l2;
  uint32_t kwh_import_l3;
}ENERGY_RECORD;



#endif /* ENERGY_DB_WD_H_ */
