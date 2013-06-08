/* Processed by ecpg (4.7.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "writeToDB.pgc"
/*#include <pgtypes_timestamp.h>*/
#include "common.h"

void
print_sqlca()
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



extern int writeToDB(const struct tm *timestamp, double temp, double hum, double pres)
{
/* exec sql whenever sql_warning  call print_sqlca ( ) ; */
#line 24 "writeToDB.pgc"

/* exec sql whenever sqlerror  call print_sqlca ( ) ; */
#line 25 "writeToDB.pgc"


/* exec sql begin declare section */
/*timestamp ts;*/
 
 
 

#line 29 "writeToDB.pgc"
 double tempreture ;
 
#line 30 "writeToDB.pgc"
 double humidity ;
 
#line 31 "writeToDB.pgc"
 double presure ;
/* exec sql end declare section */
#line 32 "writeToDB.pgc"

const char FNAME[] = "writeToDB()";
char *timestring = "2010-12-31 23:59:59            ";
int year   = -1;
int month  = -1;
int day    = -1;
int hour   = -1;
int minute = -1;
int second = -1;
tempreture = temp;
humidity = hum;
presure = pres;
timestamp=timestamp;
/*ts = PGTYPEStimestamp_from_asc("2000-01-01 00:00:00", NULL);*/
year = timestamp->tm_year;
month = timestamp->tm_mon;
day = timestamp->tm_mday;
hour = timestamp->tm_hour;
minute = timestamp->tm_min;
second = timestamp->tm_sec;

fprintf(stdout, "DEBUG: %s: year=%d month=%d day=%d hour=%d min=%d sec=%d\n", FNAME, year, month, day, hour, minute, second);
fprintf(stdout, "Timestamp = %04d-%02d-%02d %02d:%02d:%02d\n", year, month, day, hour, minute, second);
fprintf(stdout, "DEBUG: %s: Temp = %2.1f\n", FNAME, temp);
fprintf(stdout, "DEBUG: %s: Hum  = %2.0f\n", FNAME, hum);
fprintf(stdout, "DEBUG: %s: Pres = %4.0f\n", FNAME, pres);


fprintf(stdout, "DEBUG: %s: Time static = %s\n", FNAME, timestring);
/* TODO: den Timestamp in einen String umwandeln, dann den String dem to_date() übergeben ! */
/*
snprintf(timestring, 18, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
fprintf(stdout, "DEBUG: %s: Time from timestamp = %s\n", FNAME, timestring);
*/

	{ ECPGconnect(__LINE__, 0, "wetterstation" , NULL, NULL , NULL, 0); 
#line 67 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 67 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 67 "writeToDB.pgc"

	fprintf(stdout, "DEBUG: after DB-Connect");
	/*exec sql BEGIN WORK;*/
	/*to_date('2010-11-30 22:05:15', 'YYYY-MM-DD HH24:MI:SS')*//*:ts*/{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into weathervalues ( time , sensor , measurement ) values ( null , 'Temperature' , $1  )", 
	ECPGt_double,&(tempreture),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 70 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 70 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 70 "writeToDB.pgc"

	{ ECPGtrans(__LINE__, NULL, "commit");
#line 71 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 71 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 71 "writeToDB.pgc"

	fprintf(stdout, "DEBUG: after first insert");
	/*to_date('2010-11-30 22:05:15', 'YYYY-MM-DD HH24:MI:SS')*//*:ts*/{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into weathervalues ( time , sensor , measurement ) values ( null , 'Humidity' , $1  )", 
	ECPGt_double,&(humidity),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 73 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 73 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 73 "writeToDB.pgc"

	{ ECPGtrans(__LINE__, NULL, "commit");
#line 74 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 74 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 74 "writeToDB.pgc"

fprintf(stdout, "DEBUG: after second insert");
	/*to_date('2010-11-30 22:05:15', 'YYYY-MM-DD HH24:MI:SS')*//*:ts*/{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into weathervalues ( time , sensor , measurement ) values ( null , 'Presure' , $1  )", 
	ECPGt_double,&(presure),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, ECPGt_EORT);
#line 76 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 76 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 76 "writeToDB.pgc"

	{ ECPGtrans(__LINE__, NULL, "commit");
#line 77 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 77 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 77 "writeToDB.pgc"

fprintf(stdout, "DEBUG: after third insert");
	/*exec sql COMMIT WORK;*/
	{ ECPGdisconnect(__LINE__, "CURRENT");
#line 80 "writeToDB.pgc"

if (sqlca.sqlwarn[0] == 'W') print_sqlca ( );
#line 80 "writeToDB.pgc"

if (sqlca.sqlcode < 0) print_sqlca ( );}
#line 80 "writeToDB.pgc"

fprintf(stdout, "DEBUG: after DB-Disconnect");
	return 0;
}
