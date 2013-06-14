/* Processed by ecpg (4.7.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "writeToDB.pgc"
#include <pgtypes_timestamp.h>
#include "common.h"

void print_sqlca()
{
	fprintf(stderr, "==== sqlca ====\n");
	fprintf(stderr, "sqlcode: %ld\n", sqlca.sqlcode);
	fprintf(stderr, "sqlerrm.sqlerrml: %d\n", sqlca.sqlerrm.sqlerrml);
	fprintf(stderr, "sqlerrm.sqlerrmc: %s\n", sqlca.sqlerrm.sqlerrmc);
	fprintf(stderr, "sqlerrd: %ld %ld %ld %ld %ld %ld\n", sqlca.sqlerrd[0],sqlca.sqlerrd[1],sqlca.sqlerrd[2],
														sqlca.sqlerrd[3],sqlca.sqlerrd[4],sqlca.sqlerrd[5]);
	fprintf(stderr, "sqlwarn: %d %d %d %d %d %d %d %d\n", sqlca.sqlwarn[0], sqlca.sqlwarn[1], sqlca.sqlwarn[2],
														sqlca.sqlwarn[3], sqlca.sqlwarn[4], sqlca.sqlwarn[5],
														sqlca.sqlwarn[6], sqlca.sqlwarn[7]);
	fprintf(stderr, "sqlstate: %5s\n", sqlca.sqlstate);
	fprintf(stderr, "===============\n");
}

extern int writeToDB(const struct tm *timest, double temp, double hum, double pres)
{
/* exec sql whenever sql_warning  call print_sqlca ( ) ; */
#line 21 "writeToDB.pgc"

/* exec sql whenever sqlerror  call print_sqlca ( ) ; */
#line 22 "writeToDB.pgc"


/* exec sql begin declare section */
	 
	   
	   
	   

#line 25 "writeToDB.pgc"
 timestamp ts ;
 
#line 26 "writeToDB.pgc"
 double t = temp ;
 
#line 27 "writeToDB.pgc"
 double h = hum ;
 
#line 28 "writeToDB.pgc"
 double p = pres ;
/* exec sql end declare section */
#line 29 "writeToDB.pgc"


char *date_string  = NULL;;
const char FNAME[] = "writeToDB()";

errno = 0;
date_string = malloc(sizeof(char) * 20);
if (date_string == NULL)
{
	fprintf(stderr, "ERROR: %s: malloc failed. EXITING! %s", FNAME, strerror(errno));
	exit(EXIT_FAILURE);
}

snprintf(date_string, 20, "%04d-%02d-%02d %02d:%02d:%02d", timest->tm_year+1900, timest->tm_mon+1, timest->tm_mday, timest->tm_hour, 
timest->tm_min, timest->tm_sec);

ts = PGTYPEStimestamp_from_asc(date_string, NULL);
free(date_string);

fprintf(stdout, "DEBUG: %s: Timestamp = %04d-%02d-%02d %02d:%02d:%02d\n", FNAME, timest->tm_year+1900, timest->tm_mon+1, timest->tm_mday, timest->tm_hour, timest->tm_min, timest->tm_sec);
fprintf(stdout, "DEBUG: %s: Temp = %2.1f\n", FNAME, temp);
fprintf(stdout, "DEBUG: %s: Hum  = %2.0f\n", FNAME, hum);
fprintf(stdout, "DEBUG: %s: Pres = %4.0f\n", FNAME, pres);

{ ECPGconnect(__LINE__, 0, "wetterstation" , "root" , NULL , NULL, 0); 
#line 53 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 53 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 53 "writeToDB.pgc"

{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into weathervalues ( time , sensor , measurement ) values ( $1  , 'Temperature' , $2  )", 
	ECPGt_timestamp,&(ts),(long)1,(long)1,sizeof(timestamp), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_double,&(t),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 54 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 54 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 54 "writeToDB.pgc"

{ ECPGtrans(__LINE__, NULL, "commit");
#line 55 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 55 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 55 "writeToDB.pgc"

{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into weathervalues ( time , sensor , measurement ) values ( $1  , 'Humidity' , $2  )", 
	ECPGt_timestamp,&(ts),(long)1,(long)1,sizeof(timestamp), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_double,&(h),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 56 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 56 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 56 "writeToDB.pgc"

{ ECPGtrans(__LINE__, NULL, "commit");
#line 57 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 57 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 57 "writeToDB.pgc"

{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into weathervalues ( time , sensor , measurement ) values ( $1  , 'Pressure' , $2  )", 
	ECPGt_timestamp,&(ts),(long)1,(long)1,sizeof(timestamp), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_double,&(p),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 58 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 58 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 58 "writeToDB.pgc"

{ ECPGtrans(__LINE__, NULL, "commit");
#line 59 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 59 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 59 "writeToDB.pgc"

{ ECPGdisconnect(__LINE__, "CURRENT");
#line 60 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 60 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 60 "writeToDB.pgc"

return 0;
}