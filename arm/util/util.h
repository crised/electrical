
#ifndef _UTIL_H_
#define _UTIL_H_
/*
 *=============================
 * initialize util library, this method
 * should be called before any util function
 * is called.
 *============================
 */
int initialize_util();

/*
 *=============================
 * Cleanup util library, this method
 * should be called when util is not
 * going to be used anymore.
 *============================
 */
int uninitialize_util();

/*
 *=============================
 * Check if the device is registered
 * and the signal level is > 4
 *============================
 */
int is_gprs_on();

/*
 *=============================
 * Turn the led green on
 *============================
 */
int turn_led_green_on();
/*
 *=============================
 * Turn the led green off
 *============================
 */
int turn_led_green_off();
/*
 *=============================
 * Turn the led red on
 *============================
 */
int turn_led_red_on();
/*
 *=============================
 * Turn the led red of
 *============================
 */
int turn_led_red_off();

#endif

