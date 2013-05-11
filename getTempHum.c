#include "common.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

void byte_to_bin(short value) {
	short i;
	char binstr[9];
	binstr[0] = '\0';
	for (i = 7; i >= 0; i--) {
		binstr[7-i] = '0' + ((value >> i) & 0x1);
	}
	puts(binstr);
	return;
}

extern int getTempHum(struct temphum* th) {
	char FNAME[] = "getTempHum";
	int fd_hyt221 = -1;
	short b1 = 0;
	short b2 = 0;
	short b3 = 0;
	short b4 = 0;
	
	errno = 0;
	if (wiringPiSetup () == -1)
		return 1;
	fd_hyt221 = wiringPiI2CSetup(0x28);
	if (fd_hyt221 == -1) {
		fprintf(stderr, "%s: Error on setting up I2C for HYT-221\n", FNAME);
		return 1;
	}
	
	/* send MR (measuring request) command */
	wiringPiI2CWrite(fd_hyt221, 0x50);
	
	/* wait some time to finish the conversion */
	delay(100);
	
	/* send DF (data fetch) command */
	wiringPiI2CWrite(fd_hyt221, 0x51);
	
	/* read 4 bytes */
	b1 = wiringPiI2CRead(fd_hyt221);
	b2 = wiringPiI2CRead(fd_hyt221);
	b3 = wiringPiI2CRead(fd_hyt221);
	b4 = wiringPiI2CRead(fd_hyt221);
		/* debug output */
		fprintf(stdout, "%s: b1= %d BIN=", FNAME, b1);
		byte_to_bin(b1);
		fprintf(stdout, "%s: b2= %d BIN=", FNAME, b2);
		byte_to_bin(b2);
		fprintf(stdout, "%s: b3= %d BIN=", FNAME, b3);
		byte_to_bin(b3);
		fprintf(stdout, "%s: b4= %d BIN=", FNAME, b4);
		byte_to_bin(b4);
	
	/* read 4 bytes alternative way*/
	b1 = wiringPiI2CReadReg8(fd_hyt221, 0x51);
	b2 = wiringPiI2CReadReg8(fd_hyt221, 0x51);
	b3 = wiringPiI2CReadReg8(fd_hyt221, 0x51);
	b4 = wiringPiI2CReadReg8(fd_hyt221, 0x51);
		/* debug output */
		fprintf(stdout, "%s: b1= %d BIN=", FNAME, b1);
		byte_to_bin(b1);
		fprintf(stdout, "%s: b2= %d BIN=", FNAME, b2);
		byte_to_bin(b2);
		fprintf(stdout, "%s: b3= %d BIN=", FNAME, b3);
		byte_to_bin(b3);
		fprintf(stdout, "%s: b4= %d BIN=", FNAME, b4);
		byte_to_bin(b4);

		if (b1 & 0x4000) {
		fprintf(stdout, "%s: stale bit (15) is set, no new value has been calculated!\n", FNAME);
	} else if (b1 & 0x8000) {
		fprintf(stdout, "%s: command mode bit (16) is set, module in command mode!\n", FNAME);
		}
	/* mask the first 2 bit, humidity is only 14 bit */
	b1 &= 0x3FFF;
	/* calculate real humidity */
	th->hum = (100 * ((b1<<8)|b2)) >> 14;
		fprintf(stdout, "%s: real Humidity= %d\n", FNAME, th->hum);
	
	/* the last 2 bit are not used, temperature is only 14 bit */
	th->temp = (((((b3<<8)|b4) >> 2) * 165) >> 14) - 40;
		fprintf(stdout, "%s: real Temperature= %d\n", FNAME, th->temp);
	
	close (fd_hyt221);
return 0;
}