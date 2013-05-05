##
## @file Makefile
## 
## @author Jonas Marosi
## 
## @date 2013-05-05
##
## @version $Revision: 1.1 $
##
## Last Modified: 2013-05-04 $Author: Roland $
##


CC := gcc
CFlags := -Wall -Wextra -Werror -pedantic -g -O3
RM := rm -f

OBJECTGETPRESS := getPres.o
OBJECTGETTEMPHUM := getTempHum.o
OBJECTMAIN := main.o
HEADERCOMMON := common.h

%.o : %.c $(HEADERCOMMON)
	$(CC) $(CFlags) -c $<

all : main
	
main : $(OBJECTMAIN) $(OBJECTGETTEMPHUM) $(OBJECTGETPRESS)
	$(CC) $(CFlags) -o $@ $^ -lwiringPi

clean:
	$(RM) main getPress getTempHum $(OBJECTMAIN) $(OBJECTGETTEMPHUM) $(OBJECTGETPRESS)

