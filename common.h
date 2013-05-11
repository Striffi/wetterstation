#include <time.h>

struct temphum{
	int temp;
	int hum;
};

extern int getTempHum(struct temphum *th);
extern double getPres();
extern void writeLCD(double value, char mode, struct tm *time); /*mode: 't' for temp, 'h' for hum and 'p' for pres*/
extern void writeLED(int mode); /*mode: siehe mode.h*/
