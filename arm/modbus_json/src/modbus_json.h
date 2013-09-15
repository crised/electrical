/*
 * modbus_json.h
 *
 *  Created on: 10 Sep 2013
 *      Author: rnicolau
 */

#define JSON_SENT_TOKEN     "#:1\n"
#define JSON_NOTSENT_TOKEN  "#:0\n"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <syslog.h>


#define JSON_BUFFER_FILE_NAME "json_buffer.txt"
#define JSON_SENDER_LOOP_CLOCK 500 //milliseconds
#define CURL_TIMEOUT 10 //seconds


#define DETAIL_READ_PERIOD    5 * 1000 * 1000 // microseconds
#define ENERGY_READ_PERIOD    3 // on every 3rd occasion a DETAIL read occur

#define REGISTERS_FROM    10679
#define NUM_OF_REGISTERS  (10730 - REGISTERS_FROM + 1)


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

/*Max timestamp string size*/
#define MAX_TIMESTAMP_SIZE 40

/*Max MODBUS error string size*/
#define MAX_MODBUS_ERR_SIZE 128


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

int modbus_json_sender_loop(FILE *stream, char* energy_url, char* error_url, int dummy_writes);
int modbus_serial_main(char* serial, char* ip, char* port, FILE *stream, int dummy_readings);

enum ENERGY_METER_PACKET_TYPE
{
  ENERGY_METER_PACK_TYPE_ENERGY,
  ENERGY_METER_PACK_TYPE_DETAILS,
  ENERGY_METER_PACK_TYPE_ERROR
};

typedef struct ENERGY_METER_PACKET_HEADER
{
  enum ENERGY_METER_PACKET_TYPE type;
  long unsigned id;
  char timestamp[MAX_TIMESTAMP_SIZE];
  size_t size;
}ENERGY_METER_PACKET_HEADER;


typedef uint16_t  ENERGY_METER_PACKET_ENERGY[1];
typedef uint16_t  ENERGY_METER_PACKET_DETAILS[NUM_OF_REGISTERS];
typedef char      ENERGY_METER_PACKET_ERROR[MAX_MODBUS_ERR_SIZE];

typedef union ENERGY_METER_PACKET_DATA
{
  uint16_t reg[1];
  ENERGY_METER_PACKET_ENERGY energy;
  ENERGY_METER_PACKET_DETAILS details;
  ENERGY_METER_PACKET_ERROR error;
}ENERGY_METER_PACKET_DATA;
