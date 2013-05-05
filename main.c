#include <stdio.h>
#include "common.h"


extern struct temphum th;

int main (void)
{
	double pressure = 0;
        struct temphum th = {0.0, 0.0};

        printf("Calling getPres() ...\n");
	pressure = getPres();
	printf("Luftdruck: %f\n", pressure);
	
        printf("Calling getTempHum() ...\n");

        if (!getTempHum(&th)) {
            printf("relative Feuchte: %f\n", th.hum);
            printf("Temperatur: %f\n", th.temp);
        } else {
        printf("getTempHum() returned an error");
        }


return 0;
}	

