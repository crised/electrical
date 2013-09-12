/*
 * modbus_json.h
 *
 *  Created on: 10 Sep 2013
 *      Author: rnicolau
 */

#define JSON_STOP_TOKEN     "#JSON_END\n"
#define JSON_SENT_TOKEN     "#:1\n"
#define JSON_NOTSENT_TOKEN  "#:0\n"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#define JSON_BUFFER_FILE_NAME "json_buffer.txt"
#define JSON_SENDER_LOOP_CLOCK (500) //milliseconds


#define DETAIL_READ_PERIOD    5 * 1000 * 1000 // microseconds
#define ENERGY_READ_PERIOD    3 // on every 3rd occasion a DETAIL read occur

#define REGISTERS_FROM    10679
#define NUM_OF_REGISTERS  10730 - 10679 + 1

/* Calculate the memory size needed for json text:
 *
 * {"voltage1":65535,"voltage2":65535,...}
 *
 * Let say a name field will be 20 char long at max, then each field requires 29 bytes.
 *
 * So let say we need 30 bytes * NUM_OF_REGISTERS at most.
 */

#define JSON_BUFFER_SIZE  30 * NUM_OF_REGISTERS

/* 10679-10684 Voltages on line 1, 2, 3; 15 min average */
#define VOLTAGE_1 0 // * 2 + REGISTERS_FROM = 10679
#define VOLTAGE_2 1 // * 2 + REGISTERS_FROM = 10681
#define VOLTAGE_3 2 // * 2 + REGISTERS_FROM = 10683

/* 10685-10690 Voltages between line 13, 23, 31; 15 min average */
#define VOLTAGE_12 3 // * 2 + REGISTERS_FROM = 10685
#define VOLTAGE_23 4 // * 2 + REGISTERS_FROM = 10687
#define VOLTAGE_31 5 // * 2 + REGISTERS_FROM = 10689

/* 10691-10696 Current in lines 1, 2, 3; 15 min average */
#define CURRENT_1 6 // * 2 + REGISTERS_FROM = 10691
#define CURRENT_2 7 // * 2 + REGISTERS_FROM = 10693
#define CURRENT_3 8 // * 2 + REGISTERS_FROM = 10695

/* 10697-10702 Active Power in lines (Watt) 1, 2, 3; 15 min average */
#define ACT_POWER_1 9 // * 2 + REGISTERS_FROM = 10697
#define ACT_POWER_2 10 // * 2 + REGISTERS_FROM = 10699
#define ACT_POWER_3 11 // * 2 + REGISTERS_FROM = 10701

/* 10705-10710 Reactive Power in lines (VAR) 1, 2, 3; 15 min average */
#define REACT_POWER_1 13 // * 2 + REGISTERS_FROM = 10705
#define REACT_POWER_2 14 // * 2 + REGISTERS_FROM = 10707
#define REACT_POWER_3 15 // * 2 + REGISTERS_FROM = 10709

/* 10721-10722 Current in Neutral (L0); 15 min average */
#define CURRENT_N 20 // * 2 + REGISTERS_FROM = 10721

/* 10723-10724 Frequency; 15 min average */
#define FREQUENCY 21 // * 2 + REGISTERS_FROM = 10723

/* 10725-10730 Apparent power in lines (VA) 1, 2, 3; 15 min average */
#define APP_POWER_1 23 // * 2 + REGISTERS_FROM = 10725
#define APP_POWER_2 24 // * 2 + REGISTERS_FROM = 10727
#define APP_POWER_3 25 // * 2 + REGISTERS_FROM = 10729

/* 7980 Active Total Energy (Wh) */
#define ACT_TOTAL_ENERGY 7980

int modbus_json_sender_loop(FILE *stream, char* URL, int dummy_writes);
int modbus_serial_main(int argc, char *args[], FILE *stream, int dummy_readings);

enum
{
  ENERGY_METER_PACK_TYPE_ENERGY,
  ENERGY_METER_PACK_TYPE_DETAILS
};

typedef union ENERGY_METER_PAYLOAD
{
  uint16_t tab_reg[1];
  uint16_t tab_reg_energy[1];
  uint16_t tab_reg_details[NUM_OF_REGISTERS];
}ENERGY_METER_PAYLOAD;

typedef struct ENERGY_METER_PACKET
{
  uint16_t type;
  char timestamp[40];
  long unsigned id;
  ENERGY_METER_PAYLOAD payload;
}ENERGY_METER_PACKET;
