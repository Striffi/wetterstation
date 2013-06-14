#include <limits.h>
#include "mode.h"
#include <lcd.h>
#include <wiringPiI2C.h>
#include "common.h"

#define CONFIG_CRITVALUES  "critvalues.config"
#define CONFIG_TIMERVALUES "timer.config"
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
const char *FNAME = "main.isCritical()";
	if (mode == 't')
	{
		fprintf(stdout, "DEBUG: %s: checking temp crit values\n", FNAME);
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
		fprintf(stdout, "DEBUG: %s: checking hum crit values\n", FNAME);
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
		fprintf(stdout, "DEBUG: %s: checking pres crit values\n", FNAME);
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
		fprintf(stdout, "DEBUG: %s: Mode does not exist\n", FNAME);
		return -1;
	}
}

void doSleep(const int sec, const int nsec)
{
	struct timespec sleeping_time;
	struct timespec *inter_sleep_time = NULL;

	sleeping_time.tv_sec = sec; 
	sleeping_time.tv_nsec = nsec;
	/*fprintf(stdout, "DEBUG: doSleep(): start nanosleep with %d sec plus %d nsec\n", sec, nsec);*/
	errno = 0;
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
	const char *FNAME = "main.getTime()";
	time_t now;
	time(&now);
	timeofday = localtime(&now);
	fprintf(stdout, "DEBUG: %s: timeofday = %02d:%02d:%02d\n", FNAME, timeofday->tm_hour, timeofday->tm_min, timeofday->tm_sec);
	return timeofday;
}

void checkstrtolErrors(long *value)
{
	const char *FNAME = "main.checkstrtolErrors()";
	errno = 0;
	if ((errno == ERANGE && (*value == LONG_MAX || *value == LONG_MIN)) || (errno != 0 && *value == 0)) 
	{
		fprintf(stderr, "ERROR: %s: %s\n", FNAME, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

void readConfig(const char *file, const int flag, long *sec, long *nsec, long *pers_interval)
{
	const char *FNAME = "main.readConfig()";
	FILE *config;
	char buf[80];
	char *help;
	int i = 0;
	char *endptrstrtol = NULL;
	
	fprintf(stdout, "DEBUG: %s: try to open file: %s\n", FNAME, file);
	errno = 0;
	if((config = fopen(file, "r")) != NULL)
	{
		while (fgets(buf,80,config) != NULL)
		{
			fprintf(stdout, "DEBUG: %s: read line %d\n", FNAME, i);
			/* check if first character is a # */
			if (buf[0] == '#')
			{
				fprintf(stdout, "DEBUG: %s: found commentary line in config file %s\n", FNAME, file);
				continue;
			}
			/* find position of the character = */
			if ((help = strchr(buf, '=')) != NULL)
			{
				/* flag 0, means to read the critical values */
				if (flag == 0)
				{
					errno = 0;
					if (i<8)
					{
						PRESVALUES[i] = strtol(help+1, &endptrstrtol, 10);
						checkstrtolErrors(&PRESVALUES[i]);
						fprintf(stdout, "DEBUG: %s: flag=%d, Pressurevalue=%ld0\n", FNAME, flag, PRESVALUES[i]);
					}
					if (i>7 && i<16)
					{
						HUMVALUES[i-8] = strtol(help+1, &endptrstrtol, 10);
						checkstrtolErrors(&HUMVALUES[i-8]);
						fprintf(stdout, "DEBUG: %s: flag=%d, Humidityvalue=%ld0\n", FNAME, flag, HUMVALUES[i-8]);
					}
					if (i>15)
					{
						TEMPVALUES[i-16] = strtol(help+1, &endptrstrtol, 10);
						checkstrtolErrors(&TEMPVALUES[i-16]);
						fprintf(stdout, "DEBUG: %s: flag=%d, Temperaturevalue=%ld0\n", FNAME, flag, TEMPVALUES[i-16]);
					}

					if (endptrstrtol == help+1)
					{
						fprintf(stderr, "ERROR: %s: config file %s incorrect line number %d: no number after = found)\n", FNAME, file, i);
						/* close the config file */
						errno = 0;
						if (fclose(config) != 0)
						{
							fprintf(stderr, "ERROR: %s: failed to close config file named %s. %s\n", FNAME, file, strerror(errno));
						}
						exit(EXIT_FAILURE);
					}
				}
				/* flag not 0, means to read the timer values */
				else
				{
					switch(i)
					{
						errno = 0;
						case 0:{
							*sec = strtol(help+1, &endptrstrtol, 10);
							checkstrtolErrors(sec);
							fprintf(stdout, "DEBUG: %s: config file %s: intervall between measurements is %ld seconds.\n", FNAME, file, *sec);
							if (endptrstrtol == help+1)
							{
								fprintf(stderr, "ERROR: %s: config file %s incorrect line number %d: no number after = found)\n", FNAME, file, i);
								exit(EXIT_FAILURE);
							}
							break;
						}
						case 1:{
							*nsec = strtol(help+1, &endptrstrtol, 10);
							checkstrtolErrors(nsec);
							fprintf(stdout, "DEBUG: %s: config file %s: intervall between measurements is %ld nanoseconds.\n", FNAME, file, *nsec);
							if (endptrstrtol == help+1)
							{
								fprintf(stderr, "ERROR: %s: config file %s incorrect line number %d: no number after = found)\n", FNAME, file, i);
								exit(EXIT_FAILURE);
							}
							break;
						}
						case 2:{
							*pers_interval = strtol(help+1, &endptrstrtol, 10);
							checkstrtolErrors(pers_interval);
							fprintf(stdout, "DEBUG: %s: config file %s: every %ld measurement will be stored in database\n", FNAME, file, *pers_interval);
							if (endptrstrtol == help+1)
							{
								fprintf(stderr, "ERROR: %s: config file %s incorrect line number %d: no number after = found)\n", FNAME, file, i);
								exit(EXIT_FAILURE);
							}
							break;
						}
						default:{
							fprintf(stderr, "ERROR: %s: line numer %d: I am programmed to read only 3 lines in %s. Sorry!\n", FNAME, i, file);
							break;
						}
					}
				}
			}
			else
			{
				/* no = found in line */
				fprintf(stderr, "ERROR: %s: config file %s incorrect line number %d (= character is missing)\n", FNAME, file, i);
				/* close the config file */
				errno = 0;
				if (fclose(config) != 0)
				{
					fprintf(stderr, "ERROR: %s: failed to close config file named %s. %s\n", FNAME, file, strerror(errno));
				}
				exit(EXIT_FAILURE);
			}
		/* next line */
		i++;
		} /* end while */
		if (errno != 0)
		{
			/* error while fgets*/
			fprintf(stderr, "ERROR: %s: error while fgets. %s\n", FNAME, strerror(errno));
			if (fclose(config) != 0)
				{
					fprintf(stderr, "ERROR: %s: failed to close config file named %s. %s\n", FNAME, file, strerror(errno));
				}
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/*error opening config-file*/
		fprintf(stderr, "ERROR: %s: cannot open the config file named %s. %s\n", FNAME, file, strerror(errno));
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "DEBUG: %s: finished reading config file named %s\n", FNAME, file);
	/* close the config file */
	errno = 0;
	if (fclose(config) != 0)
	{
		fprintf(stderr, "ERROR: %s: failed to close config file named %s. %s\n", FNAME, file, strerror(errno));
	}
}

int init_wipi_lcd(void)
{
	const char *FNAME = "main.init_wipi_lcd()";
	int fd = -1;
	/* intitialize WiringPi */
	fprintf(stdout, "DEBUG: %s: initialize WiringPi\n", FNAME);
	if (wiringPiSetup () == -1) {
		fprintf(stderr, "ERROR: %s: cannot initialize WiringPi\n", FNAME);
		return -1;
	}
	/* initialize LCD: rows, columns, bit, wiringpi pin for RS, E, data4, data5, data6, data7 */
	fd = lcdInit (2, 16, 4, 11, 10, 6, 5, 4, 1, 0, 0, 0, 0);
	if (fd == -1) {
		fprintf(stdout, "ERROR: %s: failed\n", FNAME);
		return -1;
	}
	return fd;
}

int wiringPII2C(const int address)
{
	const char *FNAME = "main.wiringPIsetup()";
	int fd = -1;
	/* initialize i2c to get file descriptor */
	errno = 0;
	fd = wiringPiI2CSetup(address);
	if (fd == -1) {
		fprintf(stderr, "ERROR: %s: failed to get fd for I2C address %d. %s\n", FNAME, address, strerror(errno));
		return -1;
	}
	return fd;
}

int main()
{
	const char FNAME[] = "main()";
	double pres = 0.0;
	struct temphum th;
	struct tm timeofday;
	long  sec = 0;
	long nsec = 0;
	long pers_interval = 0;
	int helpI = 0;
	int fd_lcd = -1;
	int fd_bosch = -1;

	/* read the config files for critical levels */
	readConfig(CONFIG_CRITVALUES, 0, NULL, NULL, NULL);

	/* read the config file for timers */
	readConfig(CONFIG_TIMERVALUES, 1, &sec, &nsec, &pers_interval);

	/* initialize the LCD */
	errno = 0;
	fd_lcd = init_wipi_lcd();
	if (fd_lcd == -1)
	{
		fprintf(stderr, "ERROR: %s: cannot access the LCD.\n", FNAME);
		exit(EXIT_FAILURE);
	}

	/* initialize the Bosch BMP180 on address 0x77 (decimal 119)*/
	fd_bosch = wiringPII2C(119);
	if (fd_bosch == -1)
	{
		fprintf(stderr, "ERROR: %s: cannot access the BMP180 via I2C address 0x77.\n", FNAME);
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		helpI++;
		if (getTempHum(&th) != 0) {
			fprintf(stderr, "ERROR: %s: getTempHum returned an error!\n", FNAME);
		}

		if ((pres = getPres(fd_bosch)) < 0)
		{
			fprintf(stderr, "ERROR: %s: getPres returned an error!\n", FNAME);
		}

		timeofday = *getTime(&timeofday);

		if (helpI == pers_interval)
		{
			fprintf(stdout, "DEBUG: %s: before writeToDB\n", FNAME);
			if (writeToDB(&timeofday, th.temp, th.hum, pres) != 0)
			{
				fprintf(stderr, "ERROR: %s: write to DB failed\n", FNAME);
			}
			helpI = 0;
		}

		writeLED(isCritical(th.temp, 't'));
		writeLCD(fd_lcd, th.temp, 't', &timeofday);
		doSleep(sec, nsec);

		writeLED(isCritical(th.hum, 'h'));
		writeLCD(fd_lcd, th.hum, 'h', &timeofday);
		doSleep(sec, nsec);

		writeLED(isCritical(pres, 'p'));
		writeLCD(fd_lcd, pres, 'p', &timeofday);
		doSleep(sec, nsec);
	}
	return 0;
}