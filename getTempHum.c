#include "common.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>

extern int getTempHum(struct temphum* th) {
	
	int fd_hyt221 = -1;
        short b1 = 0;
        short b2 = 0;
        short b3 = 0;
        short b4 = 0;
        long hum16 = 0;
        long hum   = 0;
        long temp16 = 0;
        long temp   = 0;

	if (wiringPiSetup () == -1)
		return 1;
	fd_hyt221 = wiringPiI2CSetup(0x28);	
	if (fd_hyt221 == -1) {
		printf("Error on setting up I2C for HYT-221\n");
 		return 1;
	}

	/* send MR command */
        wiringPiI2CWriteReg8(fd_hyt221, 0x29, 0x00);
        
        /* wait some time */
        delay(200);
        
        /* send DF command and read 4 bytes */
        b1 = wiringPiI2CReadReg8(fd_hyt221, 0x28);
        b2 = wiringPiI2CReadReg8(fd_hyt221, 0x28);
        b3 = wiringPiI2CReadReg8(fd_hyt221, 0x28);
        b4 = wiringPiI2CReadReg8(fd_hyt221, 0x28);
/*
        printf("b1 %d\n", b1);
        printf("b2 %d\n", b2);
        printf("b3 %d\n", b3);
        printf("b4 %d\n", b4);
*/
        /* build 16 bit humidity variable */
        hum16  = (b1 << 8) | b2;
        hum16 &= 0x3FFF;

        /* calculate real humidity */
        hum = (100 * hum16) >> 14;
        th->hum = hum;
        printf("real Humidity: %ld\n", hum);
	
        
        /* build 16 bit temperature variable */
        temp16  = (b3 << 8) | b4;

        /* temperature is only 14 bit */
        temp = temp16 >> 2;
        th->temp = temp;
        printf("real Temperature: %ld\n", temp);

        close (fd_hyt221);
	return 0;
}
