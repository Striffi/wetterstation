#!/bin/bash

# setup GPIO pins
echo "17" > /sys/class/gpio/export
echo "27" > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio17/direction
echo "out" > /sys/class/gpio/gpio27/direction

# reset GPIO pins
echo "0" > /sys/class/gpio/gpio17/value
echo "0" > /sys/class/gpio/gpio27/value

# set GPIO pin
case "$1" in
        start | green)
                echo "LED green"
                echo "1" > /sys/class/gpio/gpio17/value
                ;;
        red | stop)
                echo "LED red"
                echo "1" > /sys/class/gpio/gpio27/value
                ;;
        kill | off)
                echo "LED off"
                ;;
        *)
                echo "Usage: $0 {green|red|off}"
                exit 1
                ;;
esac

# Clean up
echo "17" > /sys/class/gpio/unexport
echo "27" > /sys/class/gpio/unexport
