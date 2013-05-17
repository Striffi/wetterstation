#include "common.h"
#include <lcd.h>

extern int8_t writeLCD(const double value, const char mode, const struct tm *time)
{
	const char FNAME[] = "writeLCD()";
	int fd_lcd   = -1;
	char *shour  = NULL;
	char *smin   = NULL;
	char *ssec   = NULL;
	char *svalue = NULL;
	
	shour  = (char *) malloc(sizeof(char)*3 + 1);
	smin   = (char *) malloc(sizeof(char)*3 + 1);
	ssec   = (char *) malloc(sizeof(char)*3 + 1);
	svalue = (char *) malloc(sizeof(char)*6 + 1);
	
	snprintf(shour,  3, "%d", time->tm_hour);
	snprintf(smin,   3, "%d", time->tm_min);
	snprintf(ssec,   3, "%d", time->tm_sec);
	
	fprintf(stdout, "DEBUG: %s: hour=%s min=%s sec=%s value=%s\n", FNAME, shour, smin, ssec, svalue);
	
	/* get file descriptor for LCD:
	 * rows, columns, bit, wiringpi pin for RS, E, data4, data5, data6, data7
	*/
	errno = 0;
	fd_lcd = lcdInit (2, 16, 4, 11, 10, 6, 5, 4, 1, 3, 3, 3, 3);
	if (fd_lcd == -1) {
		fprintf(stdout, "ERROR: %s: lcdInit() failed: %s\n", FNAME, strerror (errno));
		return -1;
	}
	/* set cursor to home (upper left position) */
	/*lcdHome(fd_lcd);*/
	
	/* clear the display */
	/*lcdClear(fd_lcd);*/ /*macht blödsinn! nicht verwenden!*/

	/* set the cursor position: column 0-15, row 0-1*/
	lcdPosition(fd_lcd, 0, 0);
	lcdPuts(fd_lcd, shour);
	lcdPuts(fd_lcd, ":");
	lcdPuts(fd_lcd, smin);
	lcdPutchar(fd_lcd, ':');
	lcdPuts(fd_lcd, ssec);
	lcdPosition(fd_lcd, 0, 1);
	
	switch (mode) {
		case 't':
				lcdPuts(fd_lcd, "Temperatur ");
				lcdPosition(fd_lcd, 11, 1);
				snprintf(svalue, 6, "%2.1f", value);
				lcdPrintf(fd_lcd, svalue);
				break;
		case 'h':
				lcdPuts(fd_lcd, "Feuchte ");
				lcdPosition(fd_lcd, 8, 1);
				snprintf(svalue, 6, "%2.0f", value);
				lcdPrintf(fd_lcd, svalue);
				lcdPutchar(fd_lcd, '%');
				break;
		case 'p':
				lcdPuts(fd_lcd, "Druck ");
				lcdPosition(fd_lcd, 6, 1);
				snprintf(svalue, 6, "%4.0f", value);
				lcdPrintf(fd_lcd, svalue);
				lcdPuts(fd_lcd, " HPa");
				break;
	}
	
	return 0;
}
