#include "common.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>

#define I2C_ADR_BMP180 0x77
#define OSS 1 /* Oversampling_setting = 1 - > conversion time 7,5 ms */

extern double getPres() {
	const char FNAME[] = "getPres()";
	double pressure = 0;
	double p0; /* Pressure at sea level*/
	const double altitude = 172.0; /* altitude above mean sea level in meters of the pressure sensor */
	long int p = 0;
	
	short AC1 = 0;
	short AC2 = 0;
	short AC3 = 0;
	unsigned short AC4 = 0;
	unsigned short AC5= 0;
	unsigned short AC6= 0;
	short B1= 0;
	short B2= 0;
	
	short MB= 0;
	short MC= 0;
	short MD= 0;
	
	/*long int UT;*/
	
	long X1= 0;
	long X2= 0;
	long X3= 0;
	long B3= 0;
	unsigned long B4= 0;
	long B5 = 0;
	long B6= 0;
	unsigned long B7= 0;

	long MSB= 0;
	long LSB= 0;
	long XLSB= 0;
	
	long UP= 0;
	
	int fd_bosch;
	
	/* printf("Setting up wiringPi ....!\n"); */

	if (wiringPiSetup () == -1)
		return 1;
	fd_bosch = wiringPiI2CSetup(0x77);	
	if (fd_bosch == -1) {
		fprintf(stderr, "%s: Error on setting up I2C for Bosch\n", FNAME);
 		return 1;
	}

	/* Reading Calibration Data */

	AC1 = wiringPiI2CReadReg8(fd_bosch, 0xAA);
	AC1 = AC1 << 8;
	AC1 |= wiringPiI2CReadReg8(fd_bosch, 0xAB);
	if (AC1 == 0) fprintf(stdout, "%s: AC1 = 0 !\n", FNAME);
	/*if (AC1 == 0xFFFF) printf ("AC1 = 0xFFF !\n");*/

	AC2 = wiringPiI2CReadReg8(fd_bosch, 0xAC);
	AC2 = AC2 << 8;
	AC2 |= wiringPiI2CReadReg8(fd_bosch, 0xAD);
	
	AC3 = wiringPiI2CReadReg8(fd_bosch, 0xAE);
	AC3 = AC3 << 8;
	AC3 |= wiringPiI2CReadReg8(fd_bosch, 0xAF);
	
	AC4 = wiringPiI2CReadReg8(fd_bosch, 0xB0);
	AC4 = AC4 << 8;
	AC4 |= wiringPiI2CReadReg8(fd_bosch, 0xB1);
	
	AC5 = wiringPiI2CReadReg8(fd_bosch, 0xB2);
	AC5 = AC5 << 8;
	AC5 |= wiringPiI2CReadReg8(fd_bosch, 0xB3);
	
	AC6 = wiringPiI2CReadReg8(fd_bosch, 0xB4);
	AC6 = AC6 << 8;
	AC6 |= wiringPiI2CReadReg8(fd_bosch, 0xB5);
	
	B1 = wiringPiI2CReadReg8(fd_bosch, 0xB6);
	B1 = B1 << 8;
	B1 |= wiringPiI2CReadReg8(fd_bosch, 0xB7);
	
	B2 = wiringPiI2CReadReg8(fd_bosch, 0xB8);
	B2 = B2 << 8;
	B2 |= wiringPiI2CReadReg8(fd_bosch, 0xB9);
	
	MB = wiringPiI2CReadReg8(fd_bosch, 0xBA);
	MB = MB << 8;
	MB |= wiringPiI2CReadReg8(fd_bosch, 0xBB);
	
	MC = wiringPiI2CReadReg8(fd_bosch, 0xBC);
	MC = MC << 8;
	MC |= wiringPiI2CReadReg8(fd_bosch, 0xBD);
	
	MD = wiringPiI2CReadReg8(fd_bosch, 0xBE);
	MD = MD << 8;
	MD |= wiringPiI2CReadReg8(fd_bosch, 0xBF);
	
	/*
	printf ("MD = %d\n", MD);
	printf ("AC5 = %d\n", AC5);
	printf ("Ac6 = %d\n", AC6);*/
	
	/*Start Conversion - Oversampling_setting should be 1 - standard*/
	
	wiringPiI2CWriteReg8(fd_bosch, 0xF4, (0x34 + (OSS<<6))); 
	
	if (OSS != 0) {
	delay(8*OSS); /* conversion should be finished after 7,5 ms with OSS=1 */
	}
	else {delay(5);} /* conversion should be finished after 4,5 ms with OSS=0 */

	MSB = wiringPiI2CReadReg8(fd_bosch, 0xF6);
	LSB = wiringPiI2CReadReg8(fd_bosch, 0xF7);
	XLSB = wiringPiI2CReadReg8(fd_bosch, 0xF8);
	
	UP = ((MSB<<16) + (LSB<<8) + XLSB) >> (8-OSS);
	
	/*Calculate True Pressure*/
	
	/*X1 = ((UP - AC6) * AC5) >> 15;
	X2 = (MC << 11) / (X1 + MD);
	B5 = X1 + X2;*/
	
	B6 = B5 - 4000;
	X1 = (B2 * ((B6 * B6) >> 12))>>11;
	X2 = (AC2 * B6)>>11;
	X3 = X1 + X2;
	
	B3 = (((((long)AC1) * 4 + X3)<< OSS) + 2)/4; /* ???? */
	
	X1 = (AC3 * B6)>>13;
	X2 = (B1 * (B6*B6>>12))>>16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = (AC4 * (unsigned long)(X3 + 32768)>>15);
	B7 = ((unsigned long)UP - B3) * (50000 >> OSS);
	if (B7 <  0x80000000) {
		p = (B7*2)/B4;
	}
	else {
		p = (B7 / B4 ) * 2;
	}
	
	X1 = (p>>8)*(p>>8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	pressure = p + ((X1 + X2 + 3791) >> 4);
	
	/* respecting sea level */
	p0 = (1.0 - (altitude / 44330.0));
	p0 = pressure / (pow(p0, 5.255));
	
	pressure = pressure/100.0;
	p0 = p0 / 100.0;
	
	fprintf(stdout, "DEBUG: %s: real pressure = %4.0f HPa\n", FNAME, pressure);
	fprintf(stdout, "DEBUG: %s: real pressure at sea level = %4.0f HPa\n", FNAME, p0);
	close (fd_bosch);
	return pressure;
}