#include <stdio.h>
#include "abDIO.h"
#include "abGSM.h"
#include "baseTypes.h"


int initialize_util() {
	return abGSM_PowerOn(G_GSM1, NULL) && abDIO_Initialize();
}

int uninitialize_util() {
	return abGSM_PowerOff(G_GSM1) || abDIO_Uninitialize();
}

/*
 *===========================================
 * NOTE: Before calling any of the functions 
 *       bellow, the abGSM_Initialize();
 *       should be called (called from initialize_util)
 *       And abGSM_Uninitialize(); before
 *       exiting to cleanup.
 *===========================================
 */ 

int is_gprs_on(){
	gsmStatus_t status;
	abError_t result = abGSM_ReadStatus(G_GSM1, &status);
	return (result == ABERR_OK) && (status.registrationState == REG_REGISTERED) && (status.signalQuality > 4) ? 1 : 0;
}


/*
 *===========================================
 * NOTE: Before calling any of the functions 
 *       bellow, the abDIO_Initialize();
 *       should be called (called from initialize_util)
 *       And abDIO_Uninitialize(); before
 *       exiting to cleanup.
 *===========================================
 */ 
int turn_led_on_off(int the_led, int on_off){

	switch(abDIO_Write(the_led, on_off)) {
		case ABERR_OK:
			return 0;
		case ABERR_INITERROR:
		        return -1;
		case ABERR_IOERROR:
		case ABERR_OPERROR:
		case ABERR_VALERROR:
		case ABERR_SEMERROR:
		case ABERR_PRODCHKERROR:
		case ABERR_NOHARDWARE:
		case ABERR_ATERROR:
		default:
			return 1;
	}
	return 0;
}

int turn_led_green_on(){
	return turn_led_on_off(DIOD_LEDGREEN,DIOS_ON);
}

int turn_led_green_off(){
	return turn_led_on_off(DIOD_LEDGREEN,DIOS_OFF);
}

int turn_led_red_on(){
	return turn_led_on_off(DIOD_LEDRED,DIOS_ON);
}

int turn_led_red_off(){
	return turn_led_on_off(DIOD_LEDRED,DIOS_OFF);
}

