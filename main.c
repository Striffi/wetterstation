#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "common.h"
#include "mode.h"

#define TRUE 0;
#define FALSE 1;

long PRESVALUES[8];
/*0 pres_verylow
 1 pres_low_min
 2 pres_low_max
 3 pres_normal_min
 4 pres_normal_max
 5 pres_high_min
 6 pres_high_max
 7 pres_veryhigh*/


int isCritical(double value, char mode)
{
	if (mode == 't')
	{
		/*if (value < critTempLow || value > critTempHigh)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}*/
	}
	else if (mode == 'h')
	{
		/*if (value < critHumLow || value > critHumHigh)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}*/
	}
	else if (mode == 'p')
	{
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
		} else
		{
			return PRES_VERYHIGH;
		}
	}
	else
	{
		printf("Mode does not exist");
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
	return 0;
}

struct tm *getTime(struct tm *timeofday)
{
	time_t now;
	
	time(&now);
	timeofday = localtime(&now);
	return timeofday;
}

void readConfig(char *file, int flag, long *sec, long *nsec)
{
	FILE *config;
	char buf[80];
	char *help;
	int i=0;
	char *endptrstrtol = NULL;
	
	if((config = fopen(file, "r")) != NULL)
	{
		while (fgets(buf,80,config) != NULL)
		{
			if ((help = strchr(buf, '=')) != NULL)
			{
				if (flag == 0)
				{
					errno = 0;
					PRESVALUES[i] = strtol(help+1, &endptrstrtol, 10);
					if ((errno == ERANGE && (PRESVALUES[i] == LONG_MAX || PRESVALUES[i] == LONG_MIN)) || (errno != 0 && PRESVALUES[i] == 0)) {
						fprintf(stderr, "%s\n",strerror(errno));
						exit(EXIT_FAILURE);
					}

					if (endptrstrtol == help+1) {
						fprintf(stderr, "No digits were found\n");
						exit(EXIT_FAILURE);
					}
				} 
				else
				{
					switch(i)
					{
						errno = 0;
						case 0:{ 
							*sec = strtol(help+1, &endptrstrtol, 10);
							if ((errno == ERANGE && (*sec == LONG_MAX || *sec == LONG_MIN)) || (errno != 0 && *sec == 0)) {
								fprintf(stderr, "%s\n",strerror(errno));
								exit(EXIT_FAILURE);
							}
							if (endptrstrtol == help+1) {
								fprintf(stderr, "No digits were found\n");
								exit(EXIT_FAILURE);
							}
							break;
						}
						case 1:{
							*nsec = strtol(help+1, &endptrstrtol, 10);
							if ((errno == ERANGE && (*nsec == LONG_MAX || *nsec == LONG_MIN)) || (errno != 0 && *nsec == 0)) {
								fprintf(stderr, "%s\n",strerror(errno));
								exit(EXIT_FAILURE);
							}
							if (endptrstrtol == help+1) {
								fprintf(stderr, "No digits were found\n");
								exit(EXIT_FAILURE);
							}
							break;
						}
						default:{
							fprintf(stderr, "error reading timer.config");
							break;
						}
					}
				}		
				
				fclose(config);
			}
			else
			{
				/*error searching "=" in config-file*/
				fclose(config);
				exit(EXIT_FAILURE);
			}
			i++;
		}
		if (errno != 0)
		{
			/*error while fgets*/
			fclose(config);
			exit(EXIT_FAILURE);
		}
	} 
	else
	{
		/*error opening config-file*/
		exit(EXIT_FAILURE);
	}
	
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
	
	readConfig("critvales.config", 0, NULL, NULL);
	
	readConfig("timer.config", 1, &sec, &nsec);
	
	while (1)
	{
		helpI++;
		getTempHum(&th);
		temp = th.temp;
		hum = th.hum;
		pres = getPres();
	
		timeofday = *getTime(&timeofday);
	
		/*TODO: 
		* XML parser and writer
		* */
		if (helpI == 60)
		{
			if (writeToXML(temp, hum, pres, &timeofday) != 0)
			{
				/*TODO- Errorhandling*/
			}
			helpI = 0;
		}
		
		writeLED(isCritical(temp, 't'));
		writeLCD(temp, 't', &timeofday);
		doSleep(sec, nsec);
	
		writeLED(isCritical(hum, 'h'));
		writeLCD(hum, 'h', &timeofday);
		doSleep(sec, nsec);
	
		writeLED(isCritical(pres, 'p'));
		writeLCD(pres, 'p', &timeofday);
		doSleep(sec, nsec);
	}
	return 0;
}
