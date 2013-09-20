/*
 ============================================================================
 Name        : hello.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <modbus/modbus-rtu.h>
#include <errno.h>

//256 V1 16 bit voltage

int main(void) {

	modbus_t *ctx;
	uint16_t tab_reg[64];
	int rc, i;

	ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8,1);
	modbus_set_slave(ctx, 1);
	if (ctx == NULL) {
	    fprintf(stderr, "Unable to create the libmodbus context\n");
	    return -1;
	}

	int connect = modbus_connect(ctx);

	rc = modbus_read_registers(ctx, 256, 1000, tab_reg);
	if (rc == -1) {
		printf("Read_Register: %s\n", "Error");
	    return -1;
	}


	modbus_flush(ctx);
	modbus_close(ctx);
	modbus_free(ctx);


	return EXIT_SUCCESS;

}
