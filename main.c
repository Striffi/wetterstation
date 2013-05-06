#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "common.h"
/*#include "critvalues.h"*/

#define TRUE 0;
#define FALSE 1;

/*int isCritical(double value, char mode)
{
	if (mode == 't')
	{
		if (value < critTempLow || value > critTempHigh)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if (mode == 'h')
	{
		if (value < critHumLow || value > critHumHigh)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if (mode == 'p')
	{
		if (value < critPresLow || value > critPresHigh)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		printf("Mode does not exist");
		return 1;
	}

}*/

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

int main()
{
	double temp = 0.0;
	double hum = 0.0;
	double pres = 0.0;
	struct temphum th;
	struct tm timeofday;
	int sec = 20; /*reading from properties file TODO*/
	int nsec = 0;
	int helpI = 0;
	
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
		
		writeLED(0, /*isCritical(hum, 't')*/0, 5);
		writeLCD(temp, 't', &timeofday);
		doSleep(sec, nsec);
	
		writeLED(1, /*isCritical(hum, 'h')*/0, 5);
		writeLCD(hum, 'h', &timeofday);
		doSleep(sec, nsec);
	
		writeLED(2, /*isCritical(pres, 'p')*/0, 5);
		writeLCD(pres, 'p', &timeofday);
		doSleep(sec, nsec);
	}
	return 0;
}
