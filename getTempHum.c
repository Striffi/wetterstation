#include "common.h"
#include <math.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

#define HYT221_I2C_ADDRESS 0x28
#define HYT221_MR_COMMAND  0x50
#define HYT221_DF_COMMAND  0x51

const char FNAME[] = "getTempHum()";

/* print a byte as binary string for debug info */
void byte_to_bin(const char value) {
	int i = 0;
	char binstr[9] = {0};
	for (i = 7; i >= 0; i--) {
		binstr[7-i] = '0' + ((value >> i) & 0x0001);
	}
	puts(binstr);
	return;
}

int open_I2C_device(const char *i2cdevname) {
	int fd = -1;
	errno = 0;
	/* open the I2C device */
	fd = open(i2cdevname, O_RDWR);
	if (fd == -1) {
		fprintf(stderr, "ERROR: %s: Could not open I2C device! errno: %s\n", FNAME, strerror(errno));
		return -1;
	}
	else {
		/* announce the i2c address of the sensor */
		if (ioctl(fd, I2C_SLAVE, HYT221_I2C_ADDRESS) < 0) {
			fprintf(stderr, "ERROR: %s: Cannot detect I2C Slave on this address: %x! errno: %s\n", FNAME, HYT221_I2C_ADDRESS, strerror(errno));
			return -1;
		}
	}
	return fd;
}

int close_I2C_device(const int fd) {
	errno = 0;
	/* close the I2C device */
	if (close(fd) == -1) {
		fprintf(stderr, "ERROR: %s: Could not close I2C device! errno: %s\n", FNAME, strerror(errno));
		return -1;
	}
	return 0;
}

/* read temperature and humidity */
extern int8_t getTempHum(struct temphum *const th) {
	/* I2C device name */
	const char *i2cdevname = "/dev/i2c-1";
	/* file descriptor for I2C device */
	int fd_hyt221 = -1;
	/* store number of bytes read from I2C device */
	int8_t ackread = -1;
	/* store the I2C commands and read data */
	char i2cdata[5] = {0};
	/* store the humidity */
	uint16_t hum = 0;
	/* store the temperature */
	int16_t temp = 0;
	/* i */
	int i = 0;

	/* open the I2C device */
	fd_hyt221 = open_I2C_device(i2cdevname);
	if(fd_hyt221 == -1) {
		return 1;
	}

	/* write the MR (measure request) command to the sensor and check for ACK */
	i2cdata[0] = HYT221_MR_COMMAND;
	ackread = write(fd_hyt221, i2cdata, 1);
	if (ackread != 1)
	{
		fprintf(stderr, "ERROR: %s: no ACK received, MR command probably failed!\n", FNAME);
	}

	/* close the i2cdevice */
	if(close_I2C_device(fd_hyt221) == -1) {
		return 1;
	}

	/* wait some time for conversion */
	doSleep(0, 10000000);

	/* open the I2C device */
	if(open_I2C_device(i2cdevname) == -1) {
		return 1;
	}

	/* write the DF (data fetch) command to the sensor and check for ACK */
	i2cdata[0] = HYT221_DF_COMMAND;
	ackread = write(fd_hyt221, i2cdata, 1);
	if (ackread != 1) {
		fprintf(stderr, "ERROR: %s: no ACK received, DF command probably failed!\n", FNAME);
	}
	else
	{
		/* read 4 bytes from the sensor */
		ackread = read(fd_hyt221, i2cdata, 4);
		if (ackread != 4) {
			fprintf(stderr, "ERROR: %s: received %d bytes from sensor. Expected 4 bytes!\n", FNAME, ackread);
		}
		else
		{
			/* show me the received data */
			fprintf(stdout, "DEBUG: %s: received from sensor following 4 bytes:\n", FNAME);
			for (i=0; i<4; i++) {
				byte_to_bin(i2cdata[i]);
			}
			/* check if the the sensor is in command mode */
			if (i2cdata[0] & 0x80) {
				fprintf(stderr, "ERROR: %s: command mode bit (7) is set, module in command mode!\n", FNAME);
				/* close the i2cdevice */
				if(close_I2C_device(fd_hyt221) == -1) {
					return 1;
				}
			}
			/* check if a new value was read */
			if (i2cdata[0] & 0x40) {
				fprintf(stdout, "WARNING: %s: stale bit (6) is set, no new value has been calculated!\n", FNAME);
			}
			/* get the raw humidity value from byte 1 and 2 */
			/* mask the first 2 bit, humidity is only 14 bit */
			hum = i2cdata[0] & 0x3F;
			/* concatenate the second byte */
			hum = (hum<<8) | i2cdata[1];
			/* calculate real humidity */
			th->hum = (100.0 * ((double) hum)) / 16384.0;
			fprintf(stdout, "DEBUG: %s: real Humidity= %2.0f\n", FNAME, th->hum);
			
			/* get the raw temperature value from byte 3 and 4 */
			/* the last 2 bit are not used, temperature is only 14 bit */
			temp = ((i2cdata[2]<<8) | i2cdata[3]) >> 2;
			th->temp = ((((double) temp) * 165.0) / 16384.0) - 40.0;
			fprintf(stdout, "DEBUG: %s: real Temperature= %2.1f\n", FNAME, th->temp);
		}
	}

	/* close the i2cdevice */
	if(close_I2C_device(fd_hyt221) == -1) {
		return 1;
	}

	return 0;
}