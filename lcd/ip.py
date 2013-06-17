#!/usr/bin/python

from lcd_api import LCD
from subprocess import *
from time import sleep, strftime
from datetime import datetime
import sys

cmd = "ip addr show eth0 | grep inet | awk '{print $2}' | cut -d/ -f1"

lcd = LCD()
lcd.begin(16,1)

def run_cmd(cmd):
		p = Popen(cmd, shell=True, stdout=PIPE)
		output = p.communicate()[0]
		return output

lcd.clear()

if len(sys.argv) != 2:
	sys.exit('Usage: %s up|down' % sys.argv[0])

elif sys.argv[1] == 'up':
	ipaddr = run_cmd(cmd)
	lcd.message(datetime.now().strftime('%d.%b %H:%M:%S\n'))
	lcd.message('%s' % ( ipaddr ) )
	sleep (5)

elif sys.argv[1] == 'down':
	lcd.message(datetime.now().strftime('%d.%b %H:%M:%S\n'))
	lcd.message('going down')

else:
	sys.exit('Usage: %s up|down' % sys.argv[0])