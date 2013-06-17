#!/usr/bin/python

from lcd_api import LCD
from subprocess import * 
import sys

lcd = LCD()

lcd.begin(16,1)

lcd.clear()
#for arg in sys.argv:
#    lcd.message('%s\n' % ( arg ) )

lcd.message('%s\n' % ( sys.argv[1] ) )
lcd.message('%s' % ( sys.argv[2] ) )
#lcd.message('erste Zeile\n')
#lcd.message('zweite Z\n')
#lcd.message('3. Z.')
