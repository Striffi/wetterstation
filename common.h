
struct temphum{
	double temp;
	double hum;
};

struct timestamp{
	int hour;
	int minutes;
	int seconds;
};

extern int getTempHum(struct temphum* th);
extern double getPres();
extern void writeLCD(double value, char mode, struct timestamp time); /*mode: 't' for temp, 'h' for hum and 'p' for pres*/
extern void writeLED(int lednum, int mode, int intervall); /*0 for first and 1 for second and 2 for both
															* mode: 0 green, 1 red, 3 orange
															* intervall: blink-intervall*/
