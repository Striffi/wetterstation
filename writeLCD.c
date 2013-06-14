#include "common.h"
#include <lcd.h>

extern int8_t writeLCD(const int fd_lcd, const double value, const char mode, const struct tm *time)
{
	const char *FNAME = "writeLCD()";
	char *shour  = NULL;
	char *smin   = NULL;
	char *ssec   = NULL;
	char *svalue = NULL;
	
	shour  = (char *) alloca(sizeof(char)*3 + 1);
	smin   = (char *) alloca(sizeof(char)*3 + 1);
	ssec   = (char *) alloca(sizeof(char)*3 + 1);
	svalue = (char *) alloca(sizeof(char)*7 + 1);
	
	snprintf(shour,  3, "%02d", time->tm_hour);
	snprintf(smin,   3, "%02d", time->tm_min);
	snprintf(ssec,   3, "%02d", time->tm_sec);
	snprintf(svalue, 7, "%4.1f", value);
	fprintf(stdout, "DEBUG: %s: hour=%s min=%s sec=%s mode=%c value=%s\n", FNAME, shour, smin, ssec, mode, svalue);

	/* switch off backlight */
	lcdDisplay(fd_lcd, 1);
	/* set the cursor position: column 0-15, row 0-1*/
	/* set cursor top row, position 0 */
	lcdPosition(fd_lcd, 0, 0);
	/* clear the screen (fill with blanks) */
	lcdPuts(fd_lcd, "                ");
	/* set cursor bottom row, position 0 */
	lcdPosition(fd_lcd, 0, 1);
	/* clear the screen (fill with blanks) */
	lcdPuts(fd_lcd, "                ");

	lcdPosition(fd_lcd, 0, 0);
	lcdPuts(fd_lcd, shour);
	lcdPutchar(fd_lcd, ':');
	lcdPuts(fd_lcd, smin);
	lcdPutchar(fd_lcd, ':');
	lcdPuts(fd_lcd, ssec);
	lcdPosition(fd_lcd, 0, 1);
	
	switch (mode) {
		case 't':
				lcdPuts(fd_lcd, "Temperatur ");
				lcdPosition(fd_lcd, 11, 1);
				snprintf(svalue, 5, "%2.1f", value);
				lcdPrintf(fd_lcd, svalue);
				break;
		case 'h':
				lcdPuts(fd_lcd, "Feuchte ");
				lcdPosition(fd_lcd, 8, 1);
				snprintf(svalue, 3, "%2.0f", value);
				lcdPrintf(fd_lcd, svalue);
				lcdPutchar(fd_lcd, '%');
				break;
		case 'p':
				lcdPuts(fd_lcd, "Druck ");
				lcdPosition(fd_lcd, 6, 1);
				snprintf(svalue, 5, "%4.0f", value);
				lcdPrintf(fd_lcd, svalue);
				lcdPuts(fd_lcd, " HPa");
				break;
	}

	return 0;
}