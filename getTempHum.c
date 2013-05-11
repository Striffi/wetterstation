#include "common.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

extern int getTempHum(struct temphum* th) {
	char FNAME[] = "getTempHum";
	int fd_hyt221 = -1;
	int b12 = 0;
	int b34 = 0;
	
	if (wiringPiSetup () == -1)
		return 1;
	fd_hyt221 = wiringPiI2CSetup(0x28);
	if (fd_hyt221 == -1) {
		fprintf(stderr, "%s: Error on setting up I2C for HYT-221\n", FNAME);
		return 1;
	}
	
	/* send MR (measuring request) command */
	wiringPiI2CWriteReg8(fd_hyt221, 0x28, 0x50);
	
	/* wait some time to take place the conversion */
	delay(200);
	
	/* send DF (data fetch) command */
	/* wiringPiI2CWriteReg8(fd_hyt221, 0x28, 0x51); */
	
	/* read 4 bytes */
	b12 = wiringPiI2CReadReg16(fd_hyt221, 0x51);
	b34 = wiringPiI2CReadReg16(fd_hyt221, 0x53);
		/* debug output */
		fprintf(stdout, "%s: b12= %d\n", FNAME, b12);
		fprintf(stdout, "%s: b34= %d\n", FNAME, b34);
	if (b12 & 0x4000) {
		fprintf(stdout, "%s: stale bit (15) is set, no new value has been calculated!\n", FNAME);
	} else if (b12 & 0x8000) {
		fprintf(stdout, "%s: command mode bit (16) is set, module in command mode!\n", FNAME);
		}
	/* mask the first 2 bit, humidity is only 14 bit */
	b12 &= 0x3FFF;
	/* calculate real humidity */
	th->hum = (100 * b12) >> 14;
		fprintf(stdout, "%s: real Humidity= %d\n", FNAME, th->hum);
	
	/* the last 2 bit are not used, temperature is only 14 bit */
	th->temp = (((b34 >> 2) * 165) >> 14) - 40;
		fprintf(stdout, "%s: real Temperature= %d\n", FNAME, th->temp);
	
	close (fd_hyt221);
return 0;
}