/*
 * Author: Anibal Fernandez - 11 Sept 2013
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

/* The name of this program. */
const char* program_name;

/* Prints usage information for this program to STREAM (typically
 * stdout or stderr), and exit the program with EXIT_CODE. Does not
 * return. */
void print_usage (FILE* stream, int exit_code)
{
	fprintf (stream, "Usage: %s options\n", program_name);
	fprintf (stream,
		" -h --help Display this usage information.\n"
		" -o --turn-led-on [red/green].\n"
		" -x --turn-led-off [red/green].\n"
		" -s --gprs-status Print gprs status (means registered + signal level > 4).\n");
	exit (exit_code);
}

int main(int argc, char **argv)
{
	int next_option;
	int turn_led_on = 0, turn_led_off = 0, print_status = 0;
	const char* led_name = NULL;

	/* A string listing valid short options letters. */
	const char* const short_options = "ho:x:s";
	/* An array describing valid long options. */
	const struct option long_options[] = {
					{ "help", 0, NULL, 'h' },
					{ "turn-led-on", 1, NULL, 'o' },
					{ "turn-led-off", 1, NULL, 'x' },
					{ "gprs-status", 0, NULL, 's' },
					{ NULL, 0, NULL, 0 } /* Required at end of array. */
					};


	program_name = argv[0];
	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);
		switch (next_option) {
			case 'h': /* -h or --help */
				/* User has requested usage information. Print it to standard
				 * output, and exit with exit code zero (normal termination). */
				print_usage (stdout, 0);
			case 'o': /* -o or --turn-led-on */
				/* An argument is required, the name of the led [red/green]. */
				led_name = optarg;
				turn_led_on = 1;
				break;
			case 'x': /* -x or --turn-led-off */
				/* An argument is required, the name of the led [red/green]. */
				led_name = optarg;
				turn_led_off = 1;
				break;
			case 's': /* -s or --gprs-status */
				print_status = 1;
				break;
			case '?': /* The user specified an invalid option. */
				/* Print usage information to standard error, and exit with exit
				 * code one (indicating abnormal termination). */
				print_usage (stderr, 1);
				case -1: /* Done with options. */
			break;
			default: /* Something else: unexpected. */
				abort ();
		}
	}
	while (next_option != -1);


	/*
 	 *========================================
	 * If we got this far, is ok to init utils
	 *========================================
	 */
	if(initialize_util() != 0) {
		fprintf(stderr, "Failed to init utils library");
		exit(1);
	}
	  
	if( turn_led_on ) {
		if( strcmp( led_name, "red" ) == 0 )
			turn_led_red_on();	
		else
			turn_led_green_on();	
	}
	if( turn_led_off ) {
		if( strcmp( led_name, "red" ) == 0 )
			turn_led_red_off();	
		else
			turn_led_green_off();	
	}
		
	if( print_status )
		fprintf(stdout, "GPRS status %s", is_gprs_on() ? "ON" : "OFF" );
		
	uninitialize_util();
	return 0;
}

