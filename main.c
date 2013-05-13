#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "common.h"
#include "mode.h"

long PRESVALUES[8];
long HUMVALUES[8];
long TEMPVALUES[8];
/*0 xxx_verylow
 1 xxx_low_min
 2 xxx_low_max
 3 xxx_normal_min
 4 xxx_normal_max
 5 xxx_high_min
 6 xxx_high_max
 7 xxx_veryhigh*/


int isCritical(double value, char mode)
{
	if (mode == 't')
	{
		printf("DEBUG: checking temp crit values\n");
		if (value < TEMPVALUES[0])
		{
			return TEMP_VERYLOW;
		}
		else if(value > TEMPVALUES[1] && value < TEMPVALUES[2])
		{
			return TEMP_LOW;
		}
		else if(value > TEMPVALUES[3] && value < TEMPVALUES[4])
		{
			return TEMP_NORMAL;
		}
		else if (value > TEMPVALUES[5] && value < TEMPVALUES[6])
		{
			return TEMP_HIGH;
		} 
		else
		{
			return TEMP_VERYHIGH;
		}
	}
	else if (mode == 'h')
	{
		printf("DEBUG: checking hum crit values\n");
		if (value < HUMVALUES[0])
		{
			return HUM_VERYLOW;
		}
		else if(value > HUMVALUES[1] && value < HUMVALUES[2])
		{
			return HUM_LOW;
		}
		else if(value > HUMVALUES[3] && value < HUMVALUES[4])
		{
			return HUM_NORMAL;
		}
		else if (value > HUMVALUES[5] && value < HUMVALUES[6])
		{
			return HUM_HIGH;
		} 
		else
		{
			return HUM_VERYHIGH;
		}
	}
	else if (mode == 'p')
	{
		printf("DEBUG: checking pres crit values");
		if (value < PRESVALUES[0])
		{
			return PRES_VERYLOW;
		}
		else if(value > PRESVALUES[1] && value < PRESVALUES[2])
		{
			return PRES_LOW;
		}
		else if(value > PRESVALUES[3] && value < PRESVALUES[4])
		{
			return PRES_NORMAL;
		}
		else if (value > PRESVALUES[5] && value < PRESVALUES[6])
		{
			return PRES_HIGH;
		} 
		else
		{
			return PRES_VERYHIGH;
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
	printf("DEBUG: before nanosleep\n");
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

/*TODO*/
int writeToXML(double temp, double hum, double pres, struct tm *timeofday)
{
	temp=temp;
	hum=hum;
	pres=pres;
	timeofday=timeofday;
	printf("DEBUG: func writeToXML\n");
	return 0;
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

void readConfig(char *file, int flag, long *sec, long *nsec)
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

int main()
{
	double temp = 0.0;
	double hum = 0.0;
	double pres = 0.0;
	struct temphum th;
	struct tm timeofday;
	long sec = 0;
	long nsec = 0;
	int helpI = 0;
	
	printf("DEBUG: before readConfig the first\n");
	readConfig("critvales.config", 0, NULL, NULL);
	
	printf("DEBUG: before readConfig the second\n");
	readConfig("timer.config", 1, &sec, &nsec);
	
	while (1)
	{
		helpI++;
		printf("DEBUG: before getTempHum\n");
		getTempHum(&th);
		temp = th.temp;
		hum = th.hum;
		printf("DEBUG: before getPres\n");
		pres = getPres();
	
		printf("DEBUG: before getTime\n");
		timeofday = *getTime(&timeofday);
	
		/*TODO: 
		* XML parser and writer
		* */
		if (helpI == 60)
		{
			printf("DEBUG: before writeToXML\n");
			if (writeToXML(temp, hum, pres, &timeofday) != 0)
			{
				/*TODO- Errorhandling*/
			}
			helpI = 0;
		}
		
		printf("DEBUG: before writeLED-Temp\n");
		writeLED(isCritical(temp, 't'));
		printf("DEBUG: before writeLCD-Temp\n");
		writeLCD(temp, 't', &timeofday);
		printf("DEBUG: before doSleep-Temp\n");
		doSleep(sec, nsec);
	
		printf("DEBUG: before writeLED-Hum\n");
		writeLED(isCritical(hum, 'h'));
		printf("DEBUG: before writeLCD-Hum\n");
		writeLCD(hum, 'h', &timeofday);
		printf("DEBUG: before doSleep-Hum\n");
		doSleep(sec, nsec);
	
		printf("DEBUG: before writeLED-Temp\n");
		writeLED(isCritical(pres, 'p'));
		printf("DEBUG: before writeLCD-Temp\n");
		writeLCD(pres, 'p', &timeofday);
		printf("DEBUG: before doSleep-Temp\n");
		doSleep(sec, nsec);
	}
	return 0;
}
