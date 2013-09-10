
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <modbus-rtu.h>

/* 10.679­84: Voltages of Line 1, 2, 3. 15’ average.(this should be Phase to Neutral measures)

10.685­90: Voltages between line 1­3, 2­3, 3­1.15’ average.
10.691­96: Current in lines 1,2,3. 15’ average.
10.697­02: Active power in lines 1,2,3. (Watt) 15’ average. (Divide by 1000, to get KW)
10.705­12: Reactive power in lines 1,2,3. (VAR) 15’ average.(Divide by 1000, to get KVAR)
10.721­22: Current in Neutral (L0). 15’ Average.
10.723­24: Frequency. 15’ Average. Close to 50 Hz, almost constant.
10.725­30: Apparent power in lines 1,2,3. (VA) 15’ average.(Divide by 1000, to get KVA)

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

	uint16_t tab_reg[NUM_OF_REGISTERS] = {0};
	int rc = modbus_read_registers(ctx, REGISTERS_FROM, NUM_OF_REGISTERS, tab_reg);
	if (rc == -1) {
		fprintf(stderr, "%s\n", modbus_strerror(errno));
		exit(EXIT_FAILURE);
	}

	char json_data[JSON_BUFFER_SIZE] = {0};

	int c = snprintf(json_data, JSON_BUFFER_SIZE, "{"
			"\"voltage1\":%u,"
			"\"voltage2\":%u,"
			"\"voltage3\":%u,"
			"\"voltage13\":%u,"
			"\"voltage23\":%u,"
			"\"voltage31\":%u"
			"}",
			tab_reg[VOLTAGE_1],
			tab_reg[VOLTAGE_2],
			tab_reg[VOLTAGE_3],
			tab_reg[VOLTAGE_12],
			tab_reg[VOLTAGE_23],
			tab_reg[VOLTAGE_31]
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

	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}

