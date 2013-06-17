#!/bin/bash
echo "17" > /sys/class/gpio/export
echo "27" > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio17/direction
echo "out" > /sys/class/gpio/gpio27/direction

while :
do
echo "1" > /sys/class/gpio/gpio17/value
sleep $1
echo "0" > /sys/class/gpio/gpio17/value
echo "1" > /sys/class/gpio/gpio27/value
sleep $2
echo "0" > /sys/class/gpio/gpio27/value
done

# Clean up
echo "17" > /sys/class/gpio/unexport
echo "27" > /sys/class/gpio/unexport

