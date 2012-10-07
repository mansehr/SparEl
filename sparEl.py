#!/usr/bin/env python

import serial
import time
import string
import datetime
import os

impskWh = 1000
# Milliseconds per hour * 1000 / pulses per Kwh
msecimp = 60*60*1000*1000/impskWh

def funcDelta(d):
	"Calculates the current effect" 
	delta = float(d) # float because of division
	currentEffect = 0
	unit = 'W'
	if delta > 0:
		currentEffect = msecimp / delta

	# Print to file
	t = datetime.datetime.now()
	f = open('data.txt', 'a')
	f.write(str(t))
	f.write(';')
	f.write(str(currentEffect))
	f.write('\n');
	f.close()

	#Print to screen
	if currentEffect > 1000:
		currentEffect /= 1000
		unit = 'kW'

	print "Nuvarande: {0:.3f}{1}".format(currentEffect, unit )


def funcBuckets(b):
	"Calculates the effect per minute"
	print 'Snitt: {0:3d}kWh'.format(int(b)*60)

	# Print to file
        t = datetime.datetime.now()
	f = open('blipsperminute.txt', 'a')
        f.write(str(t))
        f.write(';')
        f.write(str(b))
        f.close()

fifoName = "/tmp/sparElFIFO"
if not os.path.exists(fifoName):
	os.mkfifo(fifoName)
dataFifo = os.open(fifoName, os.O_NONBLOCK | os.O_RDONLY )

ser = serial.Serial("/dev/ttyACM0", 115200)
ser.timeout = 1
blink = 1

while ser.isOpen():
	print "Blink:", blink
	ser.write(chr(blink))
	blink = 1-blink
	try:
		line = ser.readline()
		print "Line: ",line
		data = string.split(line, ':')
		if data[0] == 'd':
			funcDelta(data[1])
		elif data[0] == 'b':
			funcBuckets(data[1])
		else:
			print 'Unhandled data {}'.format(line)
	except ValueError as detail:
		print "Error parsning value: ", detail
