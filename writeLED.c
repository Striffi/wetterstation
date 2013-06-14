#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "common.h"

int LED_mode = 0;

/* 1 - red fast blinking - very low
 * 2 - red slowly blinking - low
 * 3 - red green alternating slowly - normal
 * 4 - green slowly blinking - high
 * 5 - green fast blinking - very high

 * everything else: red and green simultansously blinking - ERROR
*/

void* LED_output() {

pinMode(0,OUTPUT);
pinMode(2,OUTPUT);
digitalWrite(0,0);
digitalWrite(2,0);

while (1) {

switch (LED_mode) {

	case 1:
		pinMode(2,OUTPUT);
		digitalWrite(2,1);
		doSleep(0,250000000);
		digitalWrite(2,0);
		doSleep(0,250000000);
		break;

	case 2:
		pinMode(2,OUTPUT);
		digitalWrite(2,1);
		doSleep(0,500000000);
		digitalWrite(2,0);
		doSleep(0,250000000);
		break;

	case 3:
		pinMode(0,OUTPUT);
		pinMode(2,OUTPUT);
		digitalWrite(0,1);
		doSleep(0,500000000);
		digitalWrite(0,0);
		doSleep(0,500000000);
		digitalWrite(2,1);
		doSleep(0,500000000);
		digitalWrite(2,0);
		doSleep(0,500000000);
		break;

	case 4:
		pinMode(0,OUTPUT);
		digitalWrite(0,1);
		doSleep(0,250000000);
		digitalWrite(0,0);
		doSleep(0,250000000);
		break;

	case 5:
		pinMode(0,OUTPUT);
		digitalWrite(0,1);
		doSleep(0,500000000);
		digitalWrite(0,0);
		doSleep(0,250000000);
		break;

	default: /*signaling ERROR*/
		pinMode(3,OUTPUT);
		digitalWrite(3,1);
		doSleep(0,750000000);
		digitalWrite(3,0);
		doSleep(0,250000000);
		break;
	}
}
return 0;
}

extern void writeLED(uint8_t mode) {
int x = -1;
const char FNAME[] = "writeLED()";
pthread_t thread;

/* start thread only if mode has changed */
if (mode != LED_mode) {

	if (mode != 0) pthread_cancel(thread); /* cancel old thread*/

	LED_mode = mode;

	x = pthread_create(&thread, NULL, &LED_output, NULL);

	if (x != 0) fprintf (stderr, "%s: Thread for LED_output did not start\n", FNAME);
	}
}