#include "common.h"
#include <wiringPiI2C.h>
#include <math.h>

#define OSS 1 /* Oversampling_setting = 1 - > conversion time 7,5 ms */

extern double getPres() {
	const char FNAME[]= "getPres()";
	double pressure = 0;
	double temperature = 0;
	double reduced_pressure = 0;
	double Tm = 0;
	/*static double p0 = 1013.25 ;*/ /* Pressure at sea level-international barometric formula*/
	
	const double altitude = 180.0; /* Altitude of Floridsdorf in meters above mean sea level */

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
	
	long raw_pressure = 0;
	long raw_temperature = 0;
	
	long X1= 0;
	long X2= 0;
	long X3= 0;
	long B3= 0;
	unsigned long B4= 0;
	long B5 = 0;
	long B6= 0;
	unsigned long B7= 0;

	unsigned short MSB= 0;
	unsigned short LSB= 0;
	unsigned short XLSB= 0;
	
	long p = 0;
	long t = 0;
	
	int fd_bosch;
	
	fd_bosch = wiringPiI2CSetup(0x77);
	if (fd_bosch == -1) {
		printf("Error on setting up I2C for Bosch\n");
 		return 1;
	}

	/* Reading Calibration Data */

	AC1 = wiringPiI2CReadReg8(fd_bosch, 0xAA);
	AC1 = AC1 << 8;
	AC1 |= wiringPiI2CReadReg8(fd_bosch, 0xAB);
	if ((AC1 == 0) || (AC1 == 0xFFF)) printf("Error with data communication on AC1!\n");

	AC2 = wiringPiI2CReadReg8(fd_bosch, 0xAC);
	AC2 = AC2 << 8;
	AC2 |= wiringPiI2CReadReg8(fd_bosch, 0xAD);
	if ((AC2 == 0) || (AC2 == 0xFFF)) printf("Error with data communication on AC2!\n");

	AC3 = wiringPiI2CReadReg8(fd_bosch, 0xAE);
	AC3 = AC3 << 8;
	AC3 |= wiringPiI2CReadReg8(fd_bosch, 0xAF);
	if ((AC3 == 0) || (AC3 == 0xFFF)) printf("Error with data communication on AC3!\n");

	AC4 = wiringPiI2CReadReg8(fd_bosch, 0xB0);
	AC4 = AC4 << 8;
	AC4 |= wiringPiI2CReadReg8(fd_bosch, 0xB1);
	if ((AC4 == 0) || (AC4 == 0xFFF)) printf("Error with data communication on AC4!\n");

	AC5 = wiringPiI2CReadReg8(fd_bosch, 0xB2);
	AC5 = AC5 << 8;
	AC5 |= wiringPiI2CReadReg8(fd_bosch, 0xB3);
	if ((AC5 == 0) || (AC5 == 0xFFF)) printf("Error with data communication on AC5!\n");

	AC6 = wiringPiI2CReadReg8(fd_bosch, 0xB4);
	AC6 = AC6 << 8;
	AC6 |= wiringPiI2CReadReg8(fd_bosch, 0xB5);
	if ((AC6 == 0) || (AC6 == 0xFFF)) printf("Error with data communication on AC6!\n");

	B1 = wiringPiI2CReadReg8(fd_bosch, 0xB6);
	B1 = B1 << 8;
	B1 |= wiringPiI2CReadReg8(fd_bosch, 0xB7);
	if ((B1 == 0) || (B1 == 0xFFF)) printf("Error with data communication on B1!\n");

	B2 = wiringPiI2CReadReg8(fd_bosch, 0xB8);
	B2 = B2 << 8;
	B2 |= wiringPiI2CReadReg8(fd_bosch, 0xB9);
	if ((B2 == 0) || (B2 == 0xFFF)) printf("Error with data communication on B2!\n");

	MB = wiringPiI2CReadReg8(fd_bosch, 0xBA);
	MB = MB << 8;
	MB |= wiringPiI2CReadReg8(fd_bosch, 0xBB);
	if ((MB == 0) || (MB == 0xFFF)) printf("Error with data communication on MB!\n");

	MC = wiringPiI2CReadReg8(fd_bosch, 0xBC);
	MC = MC << 8;
	MC |= wiringPiI2CReadReg8(fd_bosch, 0xBD);
	if ((MC == 0) || (MC == 0xFFF)) printf("Error with data communication on MC!\n");

	MD = wiringPiI2CReadReg8(fd_bosch, 0xBE);
	MD = MD << 8;
	MD |= wiringPiI2CReadReg8(fd_bosch, 0xBF);
	if ((MD == 0) || (MD == 0xFFF)) printf("Error with data communication on MD!\n");

	/*Start Conversion for raw_temperature*/
	
	wiringPiI2CWriteReg8(fd_bosch, 0xF4, 0x2E);
	
	delay(5); /* conversion should be finished after 4,5ms */
	
	MSB = wiringPiI2CReadReg8(fd_bosch, 0xF6);
	LSB = wiringPiI2CReadReg8(fd_bosch, 0xF7);
	
	raw_temperature = (MSB << 8) + LSB;
	
	/*Start Conversion for raw_pressure*/
	
	wiringPiI2CWriteReg8(fd_bosch, 0xF4, (0x34 + (OSS<<6))); 
	
	switch (OSS) {
		case 0:
		   delay(5); /* conversion should be finished after 4,5 ms with OSS=0 */
		   break;
	
		case 1:
		   delay(8); /* conversion should be finished after 7,5 ms with OSS=1 */
                   break;

		case 2:
		   delay(14); /* conversion should be finished after 13,5 ms with OSS=2 */
		   break;

		case 3:
		   delay(26); /* conversion should be finished after 25,5 ms with OSS=3 */
		   break;
	
		default:
		   delay(26); /* Let´s be on the safe side .... */
	}

	
	MSB = wiringPiI2CReadReg8(fd_bosch, 0xF6);
	LSB = wiringPiI2CReadReg8(fd_bosch, 0xF7);
	XLSB = wiringPiI2CReadReg8(fd_bosch, 0xF8);
	
	raw_pressure = ((MSB<<16) + (LSB<<8) + XLSB) >> (8-OSS);

	/* Calculate True Temperature */
	
	X1 = ((raw_temperature - AC6) * AC5) >> 15;
	X2 = (MC << 11) / (X1 + MD);
	B5 = X1 + X2;
	t = (B5 + 8) >> 4;
	temperature = t/10.00;
	
	/*Calculate True Pressure*/
	
	B6 = B5 - 4000;
	X1 = (B2 * ((B6 * B6) >> 12))>>11;
	X2 = (AC2 * B6)>>11;
	X3 = X1 + X2;
	B3 = (((((AC1) * 4) + X3)<< OSS) + 2)/4; 
	X1 = (AC3 * B6)>>13;
	X2 = (B1 * ((B6*B6)>>12))>>16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = (AC4 * (unsigned long)(X3 + 32768))>>15;
	B7 = ((unsigned long)raw_pressure - B3) * (50000 >> OSS);
	
	if (B7 <  0x80000000) {
		p = (B7*2)/B4;
	}
	else {
		p = (B7 / B4 ) * 2;
	 }

	X1 = (p>>8)*(p>>8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357 * p) >> 16;
	p += (X1 + X2 + 3791) >> 4;
	pressure = p/100.0; /* Pascal -> hPA */
	
	/* calculating reduced pressure */
	
	Tm = (temperature + 273.15) + (0.00325 * altitude);
	reduced_pressure = pressure * exp((9.811 * altitude) / (287.05 * Tm));

	/* Calculating Altitude *//* international barometric formula*/
	/*altitude = 44330 * (1-(pow((pressure/reduced_pressure),(1/5.255)))); 
	printf("Höhenmeter: %f\n", altitude);*/
	fprintf(stdout, "DEBUG: %s: temperature = %2.1f Celcius\n", FNAME, temperature);
	fprintf(stdout, "DEBUG: %s: real pressure = %4.0f HPa\n", FNAME, pressure);
	fprintf(stdout, "DEBUG: %s: real pressure at sea level = %4.0f HPa\n", FNAME, reduced_pressure);


	close (fd_bosch);
	return reduced_pressure;
}
