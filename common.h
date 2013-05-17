#include <stdint.h>
#include <time.h>

struct temphum{
	int16_t temp;
	uint16_t hum;
};

extern int8_t getTempHum(struct temphum *th);
extern double getPres();
extern void writeLCD(double value, char mode, struct tm *time); /*mode: 't' for temp, 'h' for hum and 'p' for pres*/
extern void writeLED(uint8_t mode); /*mode: siehe mode.h*/
extern void doSleep(int sec, int nsec); 
