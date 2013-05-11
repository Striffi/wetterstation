#include "common.h"
#include <stdio.h>

extern void writeLCD(double value, char mode, struct tm *time)
{
	value=value;
	mode=mode;
	time=time;
	fprintf(stdout, "writeLCD not yet implemented!\n");
}
