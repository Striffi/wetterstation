#!/usr/bin/env python3
# dummy calibration values taken from datasheet
import i2c as i2c
import math
import time

address = 0x77

with i2c.I2CMaster() as bus:
	
	print ("testing communication...")
	
	read_id = bus.transaction(
		i2c.writing_bytes(address, 0xD0),
		i2c.reading(address, 1))
	id = read_id[0][0]
	print ("this value should be 0x55 (85dec) : %d" % id)
	if id == 85:
		print ("OK !")
	else:
		print ("error ! I expected 85. Exiting")
		exit

	print ("reseting sensor...")
	bus.transaction(i2c.writing_bytes(address, 0xE0, 0xB6))
	time.sleep(0.5)

	print ("Reading calibration data...")
	
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xAA),
		i2c.reading(address, 1))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xAB),
		i2c.reading(address, 2))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xAC),
		i2c.reading(address, 3))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xAD),
		i2c.reading(address, 4))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xAE),
		i2c.reading(address, 5))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xAF),
		i2c.reading(address, 6))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB0),
		i2c.reading(address, 7))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB1),
		i2c.reading(address, 8))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB2),
		i2c.reading(address, 9))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB3),
		i2c.reading(address, 10))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB4),
		i2c.reading(address, 11))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB5),
		i2c.reading(address, 12))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB6),
		i2c.reading(address, 13))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB7),
		i2c.reading(address, 14))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB8),
		i2c.reading(address, 15))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xB9),
		i2c.reading(address, 16))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xBA),
		i2c.reading(address, 17))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xBB),
		i2c.reading(address, 18))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xBC),
		i2c.reading(address, 19))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xBD),
		i2c.reading(address, 20))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xBE),
		i2c.reading(address, 21))
	time.sleep(0.1)
	read_calibration = bus.transaction(
		i2c.writing_bytes(address, 0xBF),
		i2c.reading(address, 22))

	ac1  = (read_calibration[0][0] << 8) + read_calibration[0][1]
	print ("AC1: %d" % ac1)
	ac2  = (read_calibration[0][2] << 8) + read_calibration[0][3]
	print ("AC2: %d" % ac2)
	ac3  = (read_calibration[0][4] << 8) + read_calibration[0][5]
	print ("AC3: %d" % ac3)
	ac4  = (read_calibration[0][6] << 8) + read_calibration[0][7]
	print ("AC4: %d" % ac4)
	ac5  = (read_calibration[0][8] << 8) | read_calibration[0][9]
	print ("AC5: %d" % ac5)
	ac6  = (read_calibration[0][10] << 8) + read_calibration[0][11]
	print ("AC6: %d" % ac6)
	b1  = (read_calibration[0][12] << 8) + read_calibration[0][13]
	print ("B1: %d" % b1)
	b2  = (read_calibration[0][14] << 8) + read_calibration[0][15]
	print ("B2: %d" % b2)
	mb  = (read_calibration[0][16] << 8) + read_calibration[0][17]
	print ("MB: %d" % mb)
	mc  = (read_calibration[0][18] << 8) + read_calibration[0][19]
	print ("MC: %d" % mc)
	md  = (read_calibration[0][20] << 8) | read_calibration[0][21]
	print ("MD: %d" % md)
	
	print ("setting dummy calibration values from datasheet")
	ac1 = 408
	ac2 = -72
	ac3 = -14383
	ac4 = 32741
	ac5 = 32757
	ac6 = 23153
	b1 = 6190
	b2 = 4
	mb = -32767
	mc = -8711
	md = 2868
	
	print ("reading temperature data")

	bus.transaction(i2c.writing_bytes(address, 0xF4, 0xE2))
	time.sleep(0.1)
	read_results = bus.transaction(
		i2c.writing_bytes(address, 0xF6),
		i2c.reading(address, 1))
	time.sleep(0.1)
	read_results = bus.transaction(
		i2c.writing_bytes(address, 0xF7),
			i2c.reading(address, 2))

	ut_MSB = read_results[0][0]
	ut_LSB = read_results[0][1]
	ut = (ut_MSB << 8) + ut_LSB
	print ("UT_MSB: %d" % ut_MSB)
	print ("UT_LSB: %d" % ut_LSB)
	print ("UT: %d" % ut)
	
	print ("calculating real temperature")
	x1 = (ut - ac6) * ac5 / math.pow( 2, 15 )
	print ("X1: %d" % x1)
	if x1 == 0 and md == 0:
		print ("X1 und MD sind Null, daher wird Division durch Null durchgeführt")
		print ("ich setze (X1 + MD) = 300 damit das Programm weiterläuft")
		sum_x1_md = 300.0
	else:
		sum_x1_md = x1 + md
	x2 = (mc << 11) / sum_x1_md
	print ("X2: %d" % x2)
	b5 = x1 + x2
	print ("B5: %d" % b5)
	temperature = (b5 + 8.0) / 1600.0
	print ("Tepmeratur = %02.01f °C" % temperature)
	
	print ("reading pressure data...")
	bus.transaction(i2c.writing_bytes(address, 0xF4, 0x34))
	time.sleep(0.1)
	read_results = bus.transaction(
		i2c.writing_bytes(address, 0xF6),
		i2c.reading(address, 3))
	time.sleep(0.1)
	read_results = bus.transaction(
		i2c.writing_bytes(address, 0xF7),
			i2c.reading(address, 4))
	read_results = bus.transaction(
		i2c.writing_bytes(address, 0xF8),
			i2c.reading(address, 5))

	up_MSB  = read_results[0][2]
	up_LSB  = read_results[0][3]
	up_XLSB = read_results[0][4]
	up = ((up_MSB << 16) + (up_LSB << 8) + up_XLSB) >> 8
	print ("UP_MSB: %d" % up_MSB)
	print ("UP_LSB: %d" % up_LSB)
	print ("UP_XLSB: %d" % up_XLSB)
	print ("UP: %d" % up)
	
	print ("calculating real pressure")
	b6 = b5 - 4000.0
	print ("B6: %d" % b6)
	# x1 = (b2 * ((b6 * b6) >> 12)) >> 11
	# 2^11= 2048, 2^12 = 4096, 2^16
	x1 = (b2 * ((b6 * b6) / 4096.0)) / 2048.0
	print ("X1: %d" % x1)
	# x2 = (ac2 * b6) >> 16
	x2 = (ac2 * b6) / 65536.0
	print ("X2: %d" % x2)
	# x3 = ((x1 + x2) + 2.0) >> 2
	x3 = ((x1 + x2) + 2.0) / 4.0
	print ("X3: %d" % x3)
	b3 = (((ac1 * 4.0) + x3) + 2.0) / 4.0
	x1 = ac3 * b6 / 32768.0
	x2 = (b1 * (b6 * (b6 / 4096.0))) / 65535.0
	x3 = ((x1 + x2) + 2.0) / 4.0
	
	# b4 = ac4 * (x3 + 32768.0) >> 15
	b4 = ac4 * (x3 + 32768.0) / 32768
	print ("B4: %d" % b4)
	# b7 = (up - b3) * (50000.0 >> oss)
	b7 = (up - b3) * (50000.0)
	print ("B7: %d" % b7)
	#if b7 < 0x80000000:
	if b7 < 2147483648:
		print ("B7 ist kleiner als 2147483648")
		p = (b7 * 2.0) / b4
		print ("P: %d" % p)
	else:
		print ("B7 ist größer als 2147483648")
		p = (b7 / b4) * 2.0
		print ("P: %d" % p)
	# x1 = (p >> 8) * (p >> 8)
	x1 = (p / 256.0) * (p / 256.0)
	print ("X1: %d" % x1)
	# x1 = (x1 * 3038.0) >> 16
	x1 = (x1 * 3038.0) / 65536.0
	print ("X1: %d" % x1)
	# x2 = (-7357.0 * p) >> 16
	x2 = (-7357.0 * p) / 65536.0
	print ("X2: %d" % x2)
	# p = p + ((x1 + x2 + 3791.0) >> 4)
	p = p + ((x1 + x2 + 3791.0) / 16.0)
	p = p / 100.0
	print ("real Pressure: %02.02f hPa" % p)

