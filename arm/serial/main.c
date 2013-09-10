/*
 * Author: Peter Csaszar - 10 Sept 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <modbus-rtu.h>

/* 10.679­84: Voltages of Line 1, 2, 3. 15’ average.(this should be Phase to Neutral measures)

Energy (this is measured continously), so we should refresh it at different interval:

79­80: Active Total Energy (Wh) (Divide by 1000, to get KWh)

I think energy can be reseted so we have to deal with this exception somehow.
*/

#define REGISTERS_FROM		10679
#define NUM_OF_REGISTERS	10730 - 10679 + 1

/* Calculate the memory size needed for json text:
 *
 * {"voltage1":65535,"voltage2":65535,...}
 *
 * Let say a name field will be 20 char long at max, then each field requires 29 bytes.
 *
 * So let say we need 30 bytes * NUM_OF_REGISTERS at most.
 */

#define JSON_BUFFER_SIZE	30 * NUM_OF_REGISTERS

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

void readAndSendEnergy(modbus_t * ctx, long unsigned counter)
{
	uint16_t tab_reg = 0;
	int rc = modbus_read_registers(ctx, ACT_TOTAL_ENERGY, 1, &tab_reg);
	if (rc == -1) {
		fprintf(stderr, "%s\n", modbus_strerror(errno));
		exit(EXIT_FAILURE);
	}

	time_t t = time(NULL);
	char json_data[JSON_BUFFER_SIZE] = {0};

	int c = snprintf(json_data, JSON_BUFFER_SIZE, "{"
			"\"TimeStamp\":\"%s\","
			"\"id\":%lu,"
			"\"energy\":%u"
			"}",
			asctime(gmtime(&t)),
			counter,
			tab_reg
			);
	if(c < 0) {
		fprintf(stderr, "Failed to format json data using snprintf: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(JSON_BUFFER_SIZE <= c){
		fprintf(stderr, "Buffer is not big enough (%u) to format json data. %s\n", JSON_BUFFER_SIZE, strerror(errno));
		exit(EXIT_FAILURE);
	}

	puts(json_data);
}

void readAndSendDatas(modbus_t * ctx, long unsigned counter)
{
	uint16_t tab_reg[NUM_OF_REGISTERS] = {0};
	int rc = modbus_read_registers(ctx, REGISTERS_FROM, NUM_OF_REGISTERS, tab_reg);
	if (rc == -1) {
		fprintf(stderr, "%s\n", modbus_strerror(errno));
		exit(EXIT_FAILURE);
	}

	time_t t = time(NULL);
	char json_data[JSON_BUFFER_SIZE] = {0};

	int c = snprintf(json_data, JSON_BUFFER_SIZE, "{"
			"\"TimeStamp\":\"%s\","
			"\"id\":%lu,"
			"\"voltage1\":%u,"
			"\"voltage2\":%u,"
			"\"voltage3\":%u,"
			"\"voltage13\":%u,"
			"\"voltage23\":%u,"
			"\"voltage31\":%u,"
			"\"current1\":%u,"
			"\"current2\":%u,"
			"\"current3\":%u,"
			"\"actPower1\":%u,"
			"\"actPower2\":%u,"
			"\"actPower3\":%u,"
			"\"reactPower1\":%u,"
			"\"reactPower2\":%u,"
			"\"reactPower3\":%u,"
			"\"currentN\":%u,"
			"\"frequency\":%u,"
			"\"appCurrent1\":%u,"
			"\"appCurrent2\":%u,"
			"\"appCurrent3\":%u"
			"}",
			asctime(gmtime(&t)),
			counter,
			tab_reg[VOLTAGE_1],
			tab_reg[VOLTAGE_2],
			tab_reg[VOLTAGE_3],
			tab_reg[VOLTAGE_12],
			tab_reg[VOLTAGE_23],
			tab_reg[VOLTAGE_31],
			tab_reg[CURRENT_1],
			tab_reg[CURRENT_2],
			tab_reg[CURRENT_3],
			tab_reg[ACT_POWER_1],
			tab_reg[ACT_POWER_2],
			tab_reg[ACT_POWER_3],
			tab_reg[REACT_POWER_1],
			tab_reg[REACT_POWER_2],
			tab_reg[REACT_POWER_3],
			tab_reg[CURRENT_N],
			tab_reg[FREQUENCY],
			tab_reg[APP_POWER_1],
			tab_reg[APP_POWER_2],
			tab_reg[APP_POWER_3]
			);
	if(c < 0) {
		fprintf(stderr, "Failed to format json data using snprintf: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if(JSON_BUFFER_SIZE <= c){
		fprintf(stderr, "Buffer is not big enough (%u) to format json data. %s\n", JSON_BUFFER_SIZE, strerror(errno));
		exit(EXIT_FAILURE);
	}

	puts(json_data);
}

int main(int argc, char *args[])
{
	const char * serial_device = "/dev/ttyS0";

	if(argc == 2)
		serial_device = args[1];

	modbus_t * ctx;

	ctx = modbus_new_rtu(serial_device, 115200, 'N', 8, 1);
	if (ctx == NULL) {
		fprintf(stderr, "Unable to create the libmodbus context\n");
		exit(EXIT_FAILURE);
	}

	if(modbus_connect(ctx) == -1) {
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

	long unsigned counter = 0;

	while(1){
		if(counter % 4)
			readAndSendDatas(ctx, counter++);
		readAndSendEnergy(ctx, counter++);

		usleep(1 * 1000 * 1000); // 5 seconds
	}

	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}

