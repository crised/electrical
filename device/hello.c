//Working - BulletProof
#include <modbus-rtu.h>
#include <stdio.h>
#include <errno.h>

int main()
{
modbus_t *ctx;
uint16_t tab_reg[64];
int rc;
int i;

ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8,1);
if (modbus_connect(ctx) == -1) {
fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
modbus_free(ctx);
return -1;
}

rc = modbus_set_slave(ctx, 1);
if (rc == -1) {
fprintf(stderr, "%s\n", modbus_strerror(errno));
return -1;
}

struct timeval response_timeout;
response_timeout.tv_sec = 5;
response_timeout.tv_usec = 0;

modbus_set_response_timeout(ctx, &response_timeout);

while(1){

	modbus_connect(ctx);
	rc = modbus_read_registers(ctx, 13952, 1, &tab_reg[0]);
	if (rc == -1) {
	fprintf(stderr, "%s\n", modbus_strerror(errno));
	return -1;
	}
	usleep(17000); //max transaction timing.

	rc = modbus_read_registers(ctx, 13954, 1, &tab_reg[1]);
	if (rc == -1) {
	fprintf(stderr, "%s\n", modbus_strerror(errno));
	return -1;
	}

	for (i=0; i < 2; i++) {
	printf("reg[%d]=%d (0x%X)\n", i, tab_reg[i], tab_reg[i]);
	}
	//modbus_flush(ctx);
	//modbus_close(ctx);
	usleep(10000);
	modbus_close(ctx);

}

modbus_free(ctx);
return 0;
}
