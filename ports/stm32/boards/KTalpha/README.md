# MicroPython port to STM32 MCUs

This directory contains the port of MicroPython to ST's line of STM32
microcontrollers.  Supported MCU series are: STM32F0, STM32F4, STM32F7 and
STM32L4.  Parts of the code here utilise the STM32Cube HAL library.

The officially supported boards are the line of pyboards: PYBv1.0 and PYBv1.1
(both with STM32F405), and PYBLITEv1.0 (with STM32F411).  See
[micropython.org/pyboard](http://www.micropython.org/pyboard/) for further
details.

Other boards that are supported include ST Discovery and Nucleo boards.
See the boards/ subdirectory, which contains the configuration files used
to build each individual board.

The STM32H7 series has preliminary support: there is a working REPL via
USB and UART, as well as very basic peripheral support, but some things do
not work and none of the advanced features of the STM32H7 are yet supported,
such as the clock tree.  At this point the STM32H7 should be considered as a
fast version of the STM32F7.

## KT device modules

### Temperature sensor module

#### Constructors:

.. class:: Sensor.TP()
	Create a temperature sensor object. Default device address is 0x50

#### Methods:

.. method:: TP.get_temp()
	Get the degree of temperature from temperature sensor device. return
	temperature value with string type.

.. method:: TP.get_devID
	Get the device ID of temperature sensor. return temperature sensor
	device ID with string type.


### pyb LED module

#### Constructors:

.. class:: pyb.LED(id)
	Create an LED object associated with the given LED:
	- ``id`` is the LED number, 1 - green, 2 - blue, 3 - red

#### Methods:

.. method:: LED.off()
	Turn the LED off.

.. method:: LED.on()
	Turn the LED on, to maximum intensity.

.. method:: LED.toggle()
	Toggle the LED between on (maximum intensity) and off. If the LED is
	at non-zero intensity then it is considered “on” and toggle will turn
	it off.


## Example

Create three threads to flash LED with red, green and blue color continuesly.
Create one thread to get temperature with one second interval.
Create one thread to drive double-motor with one second interval.

    import pyb
	import time
	import _thread
	from Sensor import TP
    from Driver import DMotor
	
	def led(id, dt):
		while 1:
			pyb.LED(id).toggle()
			time.sleep_ms(dt)
        
	def real_time_temp(n, dt):
		temp = TP()
		cnt = 0
		while 1:
			val = temp.get_temp()
			time.sleep_ms(dt)
			print(cnt, val)
			cnt = cnt + 1
	def run_motor(n, dt):
		motor = DMotor()
		while 1:
			val = motor.set_speed(40, 40)
			time.sleep_ms(dt)
			val = motor.set_speed(-40, -40)
			time.sleep_ms(dt)

	_thread.start_new_thread(led, (1, 1000))
	_thread.start_new_thread(led, (2, 1000))
	_thread.start_new_thread(led, (3, 1000))
	_thread.start_new_thread(real_time_temp, (1, 1000))
	_thread.start_new_thread(run_motor, (1, 1000))

