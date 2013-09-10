
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <modbus-rtu.h>

modbus_t *ctx;


int main(int argc, char *args[])
{
	const char * serial_device = "/dev/ttyS0";

	if(argc == 2)
		serial_device = args[1];

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

	/* Read 5 registers from the address 0 */
	uint16_t tab_reg[32] = {0};
	//modbus_read_registers(ctx, 0, 5, tab_reg);

	printf("Register contents: ");
	unsigned i = 0;
	for(i = 0; i < sizeof(tab_reg); i++){
		if(i) putchar(',');
		printf("%u", tab_reg[i]);
	}
	putchar('\n');

	modbus_close(ctx);
	modbus_free(ctx);

	return 0;
}

