##
## @file Makefile
## 
## @author Jonas Marosi
## 
## @date 2013-05-18
##
## @version $Revision: 1.1 $
##
## Last Modified: 2013-05-04 $Author: Roland $
##

ECPG = ecpg

%.c: %.pgc
	$(ECPG) $<

CC := gcc
CFlags := -Wall -Wextra -Werror -pedantic -g -O3
RM := rm -f

OBJECTGETPRESS := getPres.o
OBJECTGETTEMPHUM := getTempHum.o
OBJECTWRITELCD := writeLCD.o
OBJECTWRITELED := writeLED.o
OBJECTWRITETODB := writeToDB.o -I/usr/include/postgresql -lecpg
OBJECTMAIN := main.o
HEADERMODE := mode.h
HEADERCOMMON := common.h

%.o : %.c $(HEADERCOMMON) $(HEADERMODE)
	$(CC) $(CFlags) -c $<

all : main

main : $(OBJECTMAIN) $(OBJECTGETTEMPHUM) $(OBJECTGETPRESS) $(OBJECTWRITELCD) $(OBJECTWRITELED) $(OBJECTWRITETODB)
	$(CC) $(CFlags) -o $@ $^ -lwiringPi -lm -I/usr/include/postgresql -lecpg

clean:
	$(RM) main $(OBJECTMAIN) $(OBJECTGETTEMPHUM) $(OBJECTGETPRESS) $(OBJECTWRITELED) $(OBJECTWRITELCD) $(OBJECTWRITETODB)

