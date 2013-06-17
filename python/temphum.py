#!/usr/bin/env python3
import i2c as i2c
import time

address = 0x28
# 0x28 = bin0101000 = dec40
# DF command = 7 bit slave address + 0 = 0x28 = dec40
# MR command = 7 bit slave address + 1 = 0x29 = dec41

with i2c.I2CMaster() as bus:
	
	print ("send MR command")
	bus.transaction(i2c.writing_bytes(address, 0x29, 0x00))
	time.sleep(0.2)
	print ("send DF command and read 4 bytes")
	read_b1 = bus.transaction(
		i2c.writing_bytes(address, 0x28),
		i2c.reading(address, 1))
	print ("Bye 1: %d" % read_b1[0][0])
	read_b2 = bus.transaction(
		i2c.writing_bytes(address, 0x28),
		i2c.reading(address, 2))
	print ("Bye 2: %d" % read_b2[0][1])
	read_b3 = bus.transaction(
		i2c.writing_bytes(address, 0x28),
		i2c.reading(address, 3))
	print ("Bye 3: %d" % read_b3[0][2])
	read_b4 = bus.transaction(
		i2c.writing_bytes(address, 0x28),
		i2c.reading(address, 4))
	print ("Bye 4: %d" % read_b4[0][3])
	uh = (read_b1[0][0] << 8) | (read_b2[0][1])
	uh = uh & 0x3FFF
	print ("UH: %02.01f" % uh)
	# cannot rightshift float variable
	# h = (100.0 * uh) >> 14
	h = (100.0 * uh) / 16384.0
	print ("Humidity: %02.02f %%" % h)
	
	ut = ((read_b3[0][2] << 8) | read_b4[0][3]) >> 2
	print ("UT: %02.02f" % ut)
	# cannot rightshift float variable
	# temperatur = ((165.0 * ut) >> 14) - 40.0
	temperatur = ((165.0 * ut) / 16384.0) - 40.0
	print ("Temperatur: %02.02f °C" % temperatur)
	
	exit
