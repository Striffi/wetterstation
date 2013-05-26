/* Processed by ecpg (4.7.0) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "writeToDB.pgc"
#include "common.h"


extern int writeToDB(const struct tm *timestamp, double temp, double hum, double pres)
{
	{ ECPGconnect(__LINE__, 0, "wetter@localhost:5432" , NULL, NULL , NULL, 0); }
#line 6 "writeToDB.pgc"


	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into weathervalues ( time , valuetype , valueOf ) values ( timestamp , 'Tempreture' , temp )", ECPGt_EOIT, ECPGt_EORT);}
#line 8 "writeToDB.pgc"

	
	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into weathervalues ( time , valuetype , valueOf ) values ( timestamp , 'Huminity' , hum )", ECPGt_EOIT, ECPGt_EORT);}
#line 10 "writeToDB.pgc"


	{ ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "insert into weathervalues ( time , valuetype , valueOf ) values ( timestamp , 'Presure' , pres )", ECPGt_EOIT, ECPGt_EORT);}
#line 12 "writeToDB.pgc"

	
	{ ECPGdisconnect(__LINE__, "CURRENT");}
#line 14 "writeToDB.pgc"

	return 0;
}
