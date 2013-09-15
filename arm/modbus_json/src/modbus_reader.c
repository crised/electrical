/*
 * Author: Peter Csaszar - 10 Sept 2013
 */

#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include <modbus-rtu.h>
#include "modbus_json.h"


void sendModbusError(FILE *stream, const char* string, const char* code, long unsigned counter)
{
  ENERGY_METER_PACKET_HEADER header;
  ENERGY_METER_PACKET_DATA data;
  time_t t = time(NULL);

  header.type = ENERGY_METER_PACK_TYPE_ERROR;
  header.size = sizeof(ENERGY_METER_PACKET_ERROR);
  snprintf(header.timestamp, MAX_TIMESTAMP_SIZE, "%s", asctime(gmtime(&t)));
  header.id = counter;

  snprintf(data.error, MAX_MODBUS_ERR_SIZE, "%s : %s", string, code);

  fwrite(&header, sizeof(ENERGY_METER_PACKET_HEADER), 1, stream);
  fwrite(&data, header.size, 1, stream);
  fflush(stream);
}


void readAndSendEnergy(modbus_t * ctx, long unsigned counter, FILE *stream, int dummy_readings)
{
  ENERGY_METER_PACKET_HEADER header;
  ENERGY_METER_PACKET_DATA data;
  time_t t = time(NULL);

  header.type = ENERGY_METER_PACK_TYPE_ENERGY;
  header.size = sizeof(ENERGY_METER_PACKET_ENERGY);
  snprintf(header.timestamp, MAX_TIMESTAMP_SIZE, "%s", asctime(gmtime(&t)));
  header.id = counter;

  if (dummy_readings == 0) {
    int rc = modbus_read_registers(ctx, ACT_TOTAL_ENERGY, 1, data.reg);
    if (rc == -1) {
      sendModbusError(stream, "Failed to read energy register", modbus_strerror(errno), counter);
      ERR("Failed to read energy register. %s\n", modbus_strerror(errno));
    }
  }

  fwrite(&header, sizeof(ENERGY_METER_PACKET_HEADER), 1, stream);
  fwrite(&data, header.size, 1, stream);
  fflush(stream);
}

void readAndSendDetails(modbus_t * ctx, long unsigned counter, FILE *stream, int dummy_readings)
{
  ENERGY_METER_PACKET_HEADER header;
  ENERGY_METER_PACKET_DATA data;
  time_t t = time(NULL);

  header.type = ENERGY_METER_PACK_TYPE_DETAILS;
  header.size = sizeof(ENERGY_METER_PACKET_DETAILS);
  snprintf(header.timestamp, MAX_TIMESTAMP_SIZE, "%s", asctime(gmtime(&t)));
  header.id = counter;

  if (dummy_readings == 0) {
    int rc = modbus_read_registers(ctx, REGISTERS_FROM, NUM_OF_REGISTERS, data.reg);
    if (rc == -1) {
      sendModbusError(stream, "Failed to read energy register", modbus_strerror(errno), counter);
      ERR("Failed to read registers. %s\n", modbus_strerror(errno));
    }
  }

  fwrite(&header, sizeof(ENERGY_METER_PACKET_HEADER), 1, stream);
  fwrite(&data, header.size, 1, stream);
  fflush(stream);
}

modbus_t * init_modbus_serial(const char * serial_device)
{
  modbus_t * ctx;

  ctx = modbus_new_rtu(serial_device, 115200, 'N', 8, 1);
  if(ctx == NULL){
    CRIT("Unable to create the libmodbus context\n");
    exit(EXIT_FAILURE);
  }

  if(modbus_connect(ctx) == -1){
    CRIT("Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    exit(EXIT_FAILURE);
  }

  if(modbus_set_slave(ctx, 1) == -1){
    CRIT("Setting slave failed: %s\n", modbus_strerror(errno));
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
    CRIT("Unable to allocate libmodbus context\n");
    exit(EXIT_FAILURE);
  }

  if(modbus_connect(ctx) == -1){
    CRIT("Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    exit(EXIT_FAILURE);
  }

  return ctx;
}

void modbus_reader(modbus_t * ctx, FILE *stream, int dummy_readings)
{
  long unsigned counter = 0;
  int period = 0;

  while(1){
    if(((period++) % ENERGY_READ_PERIOD) == 0)
      readAndSendDetails(ctx, counter++, stream, dummy_readings);
    readAndSendEnergy(ctx, counter++, stream, dummy_readings);

    usleep(DETAIL_READ_PERIOD);
  }

  modbus_close(ctx);
  modbus_free(ctx);
}



int modbus_serial_main(char* serial, char* ip, char* port, FILE *stream, int dummy_readings)
{
  modbus_t * ctx = NULL;

  if(dummy_readings == 0)
  {
    if(serial)
      ctx = init_modbus_serial(serial);
    else
      ctx = init_modbus_tcp(ip, port);
  }

  modbus_reader(ctx, stream, dummy_readings);
  return 0;
}
