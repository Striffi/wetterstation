#!/bin/bash

gpio -g mode 17 out
gpio -g mode 27 out

case "$1" in
	start | green)
		echo "LED green"
		/usr/local/bin/gpio -g write 27 0
		/usr/local/bin/gpio -g write 17 1
		;;
	red)
		echo "LED red"
		/usr/local/bin/gpio -g write 17 0
		/usr/local/bin/gpio -g write 27 1
		;;
	stop | off)
		echo "LED off"
		/usr/local/bin/gpio -g write 17 0
		/usr/local/bin/gpio -g write 27 0
		;;
	*)
		echo "Usage: $0 {start|green|red|stop|off}"
		exit 1
		;;
esac
exit 0
