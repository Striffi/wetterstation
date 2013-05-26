#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>
#include <unistd.h>

struct temphum{
	double temp;
	double hum;
};

extern int8_t getTempHum(struct temphum  *const th);
extern double getPres();
extern int8_t writeLCD(const int fd_lcd, const double value, const char mode, const struct tm *time); /*mode: 't' for temp, 'h' for hum and 'p' for pres*/
extern void writeLED(uint8_t mode); /*mode: siehe mode.h*/
extern void doSleep(int sec, int nsec); 
extern int writeToDB(const struct tm *timestamp, double temp, double hum, double pres);
