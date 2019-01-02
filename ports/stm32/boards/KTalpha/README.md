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

## KT Sensor modules

### Temperature sensor module

#### Constructors:

`class`:: ___Sensor.TP()___
>Create a temperature sensor object. Default device address is `0x50`

#### Methods:

`method`:: ___TP.get_temp()___ 
>Get the degree of temperature from temperature sensor device. return temperature value with string type.

`method`:: ___TP.get_devID()___
>Get the device ID of temperature sensor. return temperature sensor device ID with string type.

### Button sensor module

#### Constructors:

`class`:: ___Sensor.Button()___
>Create a button sensor object. Default device address is `0x64`

#### Methods:

`method`:: ___Button.get_status()___ 
>Get the press status from button sensor device. return button press status value with 'dict' type. structure as below:
>> {<br>
>>   _status: value,_             value: 0: no press; 1: pressed <br>
>>   _count: value_               value: range 0 ~ 255, clear when exceed 255 <br>
>> }<br>

`method`:: ___Button.get_devID()___
>Get the device ID of button sensor. return button sensor device ID with string type.

## KT Driver modules

### Double motor module

#### Constructors:

`class`:: ___Driver.DMotor()___
>Create a double motor object. Default device address is `0x10`

#### Methods:

`method`:: ___DMotor.get_status()___ 
>Get the running status of both motors from double motors device. return status value with `dict` type. structure as below:
>> { _lmotor_speed: value,_       value: range 0 ~ 100<br>
>>   _lmotor_rotation: value,_    value: 0: stop; 1: move forward; 2 move backward<br>
>>   _rmotor_speed: value,_       value: range 0 ~ 100<br>
>>   _rmotor_rotation: value_ }   value: 0: stop; 1: move forward; 2 move backward<br>

`method`:: ___Dmotor.get_devID()___
>Get the device ID of double motors driver. return double motors device ID with string type.

`method`:: ___Dmotor.set_speed(lmotor_speed, lmotor_rotation, rmotor_speed, rmotor_rotation)___
>Set the speed and rotation of double motors. Return 0 as successful, otherwise error:
>> lmotor_speed:    range 0 ~ 100<br>
>> lmotor_rotation: 0: stop; 1: move forward; 2 move backward<br>
>> rmotor_speed:    range 0 ~ 100<br>
>> rmotor_rotation: 0: stop; 1: move forward; 2 move backward<br>

### LED module

#### Constructors:

`class`:: ___pyb.LED(id)___
>Create an LED object associated with the given LED:
>- ``id`` is the LED number, 1 - green, 2 - blue, 3 - red

#### Methods:

`method`:: ___LED.off()___
>Turn the LED off.

`method`:: ___LED.on()___
>Turn the LED on, to maximum intensity.

`method`:: ___LED.toggle()___
>Toggle the LED between on (maximum intensity) and off. If the LED is
>at non-zero intensity then it is considered “on” and toggle will turn
>it off.

### exLED module

#### Constructors:

`class`:: ___Driver.LED()___
>Create a exLED object. Default device address is `0x14`

#### Methods:

`method`:: ___LED.get_status()___ 
>Get the LED status of three color's value. return status value with `dict` type. structure as below:
>> {<br> 
>>   _red:   value,_       value: range 0 ~ 255, value "0" means red LED is turn off<br>
>>   _green: value,_       value: range 0 ~ 255, value "0" means green LED is turn off<br>
>>   _blue:  value_        value: range 0 ~ 255, value "0" means blue LED is turn off<br>
>> }<br>

`method`:: ___LED.get_devID()___
>Get the device ID of exLED driver. return exLED device ID with string type.

`method`:: ___LED.set_color(red, green, blue)___
>Set the red/green/blue color value for LED. Return 0 as successful, otherwise error:
>> red:    range 0 ~ 255, value "0" means turning off red LED<br>
>> green:  range 0 ~ 255, value "0" means turning off green LED<br>
>> blue:   range 0 ~ 255, value "0" means turning off blue LED<br>

## Example

refer example.py
