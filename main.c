#include <limits.h>
#include "mode.h"
#include <lcd.h>
#include <wiringPiI2C.h>
#include "common.h"

#define LED_VERY_LOW 1
#define LED_LOW 2
#define LED_NORMAL 3
#define LED_HIGH 4
#define LED_VERY_HIGH 5
#define LED_ERROR -1

long PRESVALUES[8];
long HUMVALUES[8];
long TEMPVALUES[8];

/* 0 xxx_verylow  --> LED MODE VERY LOW
   1 xxx_low_min  
   2 xxx_low_max --> LED MODE LOW
   3 xxx_normal_min
   4 xxx_normal_max -->  LED MODE NORMAL
   5 xxx_high_min
   6 xxx_high_max --> LED MODE HIGH
   7 xxx_veryhigh --> LED MODE VERY HIGH
*/ 


int isCritical(double value, char mode)
{
	if (mode == 't')
	{
		printf("DEBUG: checking temp crit values\n");
		if (value < TEMPVALUES[0])
		{
			return LED_VERY_LOW;
		}
		else if(value > TEMPVALUES[1] && value < TEMPVALUES[2])
		{
			return LED_LOW;
		}
		else if(value > TEMPVALUES[3] && value < TEMPVALUES[4])
		{
			return LED_NORMAL;
		}
		else if (value > TEMPVALUES[5] && value < TEMPVALUES[6])
		{
			return LED_HIGH;
		} 
		else if (value > TEMPVALUES[7])
                {
                        return LED_VERY_HIGH;
                }  
		else
		{
			return LED_ERROR;
		}
	}
	else if (mode == 'h')
	{
		printf("DEBUG: checking hum crit values\n");
		if (value < HUMVALUES[0])
		{
			return LED_VERY_LOW;
		}
		else if(value > HUMVALUES[1] && value < HUMVALUES[2])
		{
			return LED_LOW;
		}
		else if(value > HUMVALUES[3] && value < HUMVALUES[4])
		{
			return LED_NORMAL;
		}
		else if (value > HUMVALUES[5] && value < HUMVALUES[6])
		{
			return LED_HIGH;
		}
		else if (value > HUMVALUES[7])
                {
                        return LED_VERY_HIGH;
                }  
		else
		{
			return LED_ERROR;
		}
	}
	else if (mode == 'p')
	{
		printf("DEBUG: checking pres crit values");
		if (value < PRESVALUES[0])
		{
			return LED_VERY_LOW;
		}
		else if(value > PRESVALUES[1] && value < PRESVALUES[2])
		{
			return LED_LOW;
		}
		else if(value > PRESVALUES[3] && value < PRESVALUES[4])
		{
			return LED_NORMAL;
		}
		else if (value > PRESVALUES[5] && value < PRESVALUES[6])
		{
			return LED_HIGH;
		} 
		else if (value > PRESVALUES[7])
                {
                        return LED_VERY_HIGH;
                }  
		else
		{
			return LED_ERROR;
		}
	}
	else
	{
		printf("DEBUG: Mode does not exist\n");
		return -1;
	}

}

void doSleep(int sec, int nsec)
{
	struct timespec sleeping_time;
	struct timespec *inter_sleep_time = NULL;
	
	sleeping_time.tv_sec = sec; 
	sleeping_time.tv_nsec = nsec;
	errno = 0;
	fprintf(stdout, "DEBUG: doSleep(): start nanosleep with %d sec plus %d nsec\n", sec, nsec);
	if(nanosleep(&sleeping_time, inter_sleep_time) != 0)
	{
		if (errno == EFAULT)
			return;
		if (errno == EINVAL)
			return;
		if (errno == EINTR)
			doSleep(inter_sleep_time->tv_sec, inter_sleep_time->tv_nsec);
	}
}

struct tm *getTime(struct tm *timeofday)
{
	time_t now;
	printf("DEBUG: func getTime\n");
	time(&now);
	timeofday = localtime(&now);
	return timeofday;
}

void checkstrtolErrors(long *value)
{
	printf("DEBUG: func checkstrtolErrors\n");
	if ((errno == ERANGE && (*value == LONG_MAX || *value == LONG_MIN)) || (errno != 0 && *value == 0)) 
	{
		fprintf(stderr, "%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void readConfig(char *file, int flag, long *sec, long *nsec, long *pers_interval)
{
	FILE *config;
	char buf[80];
	char *help;
	int i=0;
	char *endptrstrtol = NULL;
	
	printf("DEBUG: before fopen\n");
	if((config = fopen(file, "r")) != NULL)
	{
		printf("DEBUG: before fgets\n");
		while (fgets(buf,80,config) != NULL)
		{
			if (buf[0] == '#')
			{
				printf("DEBUG: found commentary in config file\n");
				continue;
			}
			printf("DEBUG: %d line strchr\n", i);
			if ((help = strchr(buf, '=')) != NULL)
			{
				if (flag == 0)
				{
					errno = 0;
					if (i<8)
					{
						printf("DEBUG: %d value strtol flag=0\n", i);
						PRESVALUES[i] = strtol(help+1, &endptrstrtol, 10);
						checkstrtolErrors(&PRESVALUES[i]);
					}
					if (i>7 && i<16)
					{
						printf("DEBUG: %d value strtol flag=0\n", i);
						HUMVALUES[i-8] = strtol(help+1, &endptrstrtol, 10);
						checkstrtolErrors(&HUMVALUES[i-8]);
					}
					if (i>15)
					{
						printf("DEBUG: %d value strtol flag=0\n", i);
						TEMPVALUES[i-16] = strtol(help+1, &endptrstrtol, 10);
						checkstrtolErrors(&TEMPVALUES[i-16]);
					}

					if (endptrstrtol == help+1) {
						printf("DEBUG: No digits were found - strtol critvalues.config\n");
						exit(EXIT_FAILURE);
					}
				} 
				else
				{
					switch(i)
					{
						errno = 0;
						case 0:{ 
							printf("DEBUG: %d value strtol - flag=1\n", i);
							*sec = strtol(help+1, &endptrstrtol, 10);
							/*if ((errno == ERANGE && (*sec == LONG_MAX || *sec == LONG_MIN)) || (errno != 0 && *sec == 0)) {
								fprintf(stderr, "%s\n",strerror(errno));
								exit(EXIT_FAILURE);
							}*/
							checkstrtolErrors(sec);
							if (endptrstrtol == help+1) {
								printf("DEBUG: No digits were found - strtol timer.config\n");
								exit(EXIT_FAILURE);
							}
							break;
						}
						case 1:{
							printf("DEBUG: %d value strtol - flag=1\n", i);
							*nsec = strtol(help+1, &endptrstrtol, 10);
							/*if ((errno == ERANGE && (*nsec == LONG_MAX || *nsec == LONG_MIN)) || (errno != 0 && *nsec == 0)) {
								fprintf(stderr, "%s\n",strerror(errno));
								exit(EXIT_FAILURE);
							}*/
							checkstrtolErrors(nsec);
							if (endptrstrtol == help+1) {
								printf("DEBUG: No digits were found - strtol timer.config\n");
								exit(EXIT_FAILURE);
							}
							break;
						}
						case 2:{
							printf("DEBUG: %d value strtol - flag=1\n", i);
							*pers_interval = strtol(help+1, &endptrstrtol, 10);
							/*if ((errno == ERANGE && (*nsec == LONG_MAX || *nsec == LONG_MIN)) || (errno != 0 && *nsec == 0)) {
								fprintf(stderr, "%s\n",strerror(errno));
								exit(EXIT_FAILURE);
							}*/
							checkstrtolErrors(pers_interval);
							if (endptrstrtol == help+1) {
								fprintf(stderr, "ERROR: No digits were found - strtol timer.config\n");
								exit(EXIT_FAILURE);
							}
							break;
						}
						default:{
							printf("DEBUG: error reading timer.config");
							break;
						}
					}
				}		
			}
			else
			{
				/*error searching "=" in config-file*/
				printf("DEBUG: error searching for = \n");
				fclose(config);
				exit(EXIT_FAILURE);
			}
			i++;
		}
		if (errno != 0)
		{
			/*error while fgets*/
			printf("DEBUG: error while fgets\n");
			fclose(config);
			exit(EXIT_FAILURE);
		}
	} 
	else
	{
		/*error opening config-file*/
		printf("DEBUG: error opening config-file");
		exit(EXIT_FAILURE);
	}
	printf("DEBUG: finished reading config file\n");
	fclose(config);
	
}

int init_wipi_lcd(const char* FNAME)
{
	int fd_lcd = -1;
	/* intitialize WiringPi */
	fprintf(stdout, "DEBUG: %s: initialize WiringPi\n", FNAME);
	if (wiringPiSetup () == -1) {
		fprintf(stderr, "ERROR: %s: cannot initialize WiringPi\n", FNAME);
		return -1;
	}
	/* initialize LCD: rows, columns, bit, wiringpi pin for RS, E, data4, data5, data6, data7 */
	fd_lcd = lcdInit (2, 16, 4, 11, 10, 6, 5, 4, 1, 3, 3, 3, 3);
	if (fd_lcd == -1) {
		fprintf(stdout, "ERROR: %s: lcdInit() failed\n", FNAME);
		return -1;
	}
	return fd_lcd;
}

int init_wipi_bosch(const char* FNAME)
{
	int fd_bosch = -1;
	/* initialize i2c to Bosch BMP180 */
	fd_bosch = wiringPiI2CSetup(0x77);
	if (fd_bosch == -1) {
		fprintf(stdout, "ERROR: %s: wiringPiI2CSetup() failed\n", FNAME);
		return -1;
	}
	return fd_bosch;
}

int main()
{
	const char FNAME[] = "main()";
	double temp = 0.0;
	double hum  = 0.0;
	double pres = 0.0;
	struct temphum th;
	struct tm timeofday;
	long  sec = 0;
	long nsec = 0;
	long pers_interval = 0;
	int helpI = 0;
	int fd_lcd = -1;
	int fd_bosch = -1;
	
	/* initialize the LCD */
	fd_lcd = init_wipi_lcd(FNAME);
	if (fd_lcd == -1)
	{
		fprintf(stderr, "WARNING: %s: cannot access the LCD.\n", FNAME);
		exit(EXIT_FAILURE);
	}
	
	fprintf(stdout, "DEBUG: %s: before readConfig the first\n", FNAME);
	readConfig("critvalues.config", 0, NULL, NULL, NULL);
	
	fprintf(stdout, "DEBUG: %s: before readConfig the second\n", FNAME);
	readConfig("timer.config", 1, &sec, &nsec, &pers_interval);
	
	/* initialize the Bosch BMP180 */
	fd_bosch = init_wipi_bosch(FNAME);
	if (fd_bosch == -1)
	{
		fprintf(stderr, "WARNING: %s: cannot access the BMP180.\n", FNAME);
		exit(EXIT_FAILURE);
	}
	
	while (1)
	{
		helpI++;
		fprintf(stdout, "DEBUG: %s: before getTempHum\n", FNAME);
		if (getTempHum(&th) != 0) {
			fprintf(stderr, "ERROR: %s: getTempHum returned an error!\n", FNAME);
		}
		temp = th.temp;
		hum = th.hum;
		fprintf(stdout, "DEBUG: %s: before getPres\n", FNAME);
		if ((pres = getPres(fd_bosch)) < 0)
		{
			fprintf(stderr, "ERROR: %s: getPres returned an error!\n", FNAME);
		}
	
		fprintf(stdout, "DEBUG: %s: before getTime\n", FNAME);
		timeofday = *getTime(&timeofday);
	
		if (helpI == pers_interval)
		{
			fprintf(stdout, "DEBUG: %s: before writeToDB\n", FNAME);
			if (writeToDB(&timeofday, temp, hum, pres) != 0)
			{
				fprintf(stderr, "ERROR: %s: write to DB failed\n", FNAME);
			}
			helpI = 0;
		}
		
		fprintf(stdout, "DEBUG: %s: before writeLED-Temp\n", FNAME);
		writeLED(isCritical(temp, 't'));
		fprintf(stdout, "DEBUG: %s: before writeLCD-Temp\n", FNAME);
		writeLCD(fd_lcd, temp, 't', &timeofday);
		fprintf(stdout, "DEBUG: %s: before doSleep-Temp\n", FNAME);
		doSleep(sec, nsec);
	
		fprintf(stdout, "DEBUG: %s: before writeLED-Hum\n", FNAME);
		writeLED(isCritical(hum, 'h'));
		fprintf(stdout, "DEBUG: %s: before writeLCD-Hum\n", FNAME);
		writeLCD(fd_lcd, hum, 'h', &timeofday);
		fprintf(stdout, "DEBUG: %s: before doSleep-Hum\n", FNAME);
		doSleep(sec, nsec);
	
		fprintf(stdout, "DEBUG: %s: before writeLED-Temp\n", FNAME);
		writeLED(isCritical(pres, 'p'));
		fprintf(stdout, "DEBUG: %s: before writeLCD-Temp\n", FNAME);
		writeLCD(fd_lcd, pres, 'p', &timeofday);
		fprintf(stdout, "DEBUG: %s: before doSleep-Temp\n", FNAME);
		doSleep(sec, nsec);
	}
	return 0;
}
