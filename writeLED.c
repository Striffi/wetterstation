#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>

int LED_mode = 0;

/* 1 - red fast blinking - very bad
 * 2 - red slowly blinking - bad
 * 3 - red green alternating slowly - everything normal
 * 4 - green slowly blinking - better
 * 5 - green fast blinking - too good
*/

void* LED_output() {

while (1) {

switch (LED_mode) {
	
	case 1:
		pinMode(1,OUTPUT);
                digitalWrite(1,1);
                delay(250);
                digitalWrite(1,0);
                delay(250);
		break;

	case 2:

		pinMode(1,OUTPUT);
                digitalWrite(1,1);
                delay(1000);
                digitalWrite(1,0);
                delay(1000);
                break;
      
  	case 3:

		pinMode(0,OUTPUT);
		pinMode(1,OUTPUT);
                digitalWrite(0,1);
                delay(500);
                digitalWrite(0,0);
                delay(2000);
                digitalWrite(1,1);
                delay(500);
                digitalWrite(1,0);
                delay(2000);
                break;
	case 4:
		pinMode(0,OUTPUT);
                digitalWrite(0,1);
                delay(1000);
                digitalWrite(0,0);
                delay(1000);
								break;

	case 5:

		pinMode(0,OUTPUT);
                digitalWrite(0,1);
                delay(250);
                digitalWrite(0,0);
                delay(250);
                break;

	case -1:

                pinMode(1,OUTPUT);
                digitalWrite(1,1);
                delay(500);
                digitalWrite(1,0);
                delay(500);
                break;

	}
}
return 0;
}

extern void writeLED(int mode) {
int x = -1;
pthread_t thread;

/* start thread only if mode has changed */
if (mode != LED_mode) {
	if (mode != 0) pthread_cancel(thread); /* if existing, cancel old thread*/

	LED_mode = mode;
	
	x = pthread_create(&thread, NULL, &LED_output, NULL);

	if (x != 0) printf ("LED_output didn´t started\n");
	}
}


