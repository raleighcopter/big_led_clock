BFC 1.0 (Big Filament Clock 1.0)

![](https://github.com/raleighcopter/big_led_clock/blob/main/clock.gif)

![](https://raw.githubusercontent.com/raleighcopter/big_led_clock/main/clock.jpg)

Some notes...

this clock is 17" x8" x 2.5" overall.

uses 68mm 3v filaments sourced from aliexpress. digits are about 6.5" tall. get the gps module and bme280 sensor for the optional transmitter there too.

![](https://github.com/raleighcopter/big_led_clock/blob/main/filaments.jpg)

the clock software now has the ability to blank leading zeros and the ability to display in 12 or 24 hour format through a few configuration settings.

the clock has the option to add a couple transistors (ss8550 with 1k base resistor) and configure the software to dim the entire display during night hours or without the addition of 2 transistors, to dim the colon and turn the digits off. You can optionally omit the buck converter but it does add some display brightness control, especially if you omit the ss8550 transistors which can dim the digits with a bit of code configuration changes.

Order extra LED filaments. They're brittle, fragile and you will break more than one. Sourced from AliExpress. the digits are wired common anode (the white wire).
![](https://github.com/raleighcopter/big_led_clock/blob/main/digit_back.jpg)

Here's a picture of the front.
![](https://github.com/raleighcopter/big_led_clock/blob/main/digit.jpg)

THE 433 MHZ receiver/transmitter with functionality is also working and an optional transmitter with bme280 (recommended) or si7021 temp/rh sensor can be used to wirelessly transmit weather data to the clock. you can build the clock with the receiver and build the transmitter at a later date and the clock will only display time unless a transmitter with matching transmitter code is detected.

The 3v converter can be omitted but you won't be able to set the brightness of the LEDs if you do, so play with the value of the series resistors for the LEDs if you omit the buck converter. You want at least 100 ohm series resistors, possibly higher.
  
You can omit the DST switch and just use the hour offset pot to adjust the hour for DST.

I used 1/4" poplar for the base and painted it with some left-over hammer tone grey paint i had on hand.

I used 3er perf board to build the circuit. It's available cheapest on eBay.

The GPS module is a neo6m module from AliExpress and it provides an accurate time source. more importantly, after a power outage, the clock will come back with the correct time without the need to set the clock and it doesn't require a battery backup, just a weak gps signal.

Drill the holes for the filament wires out to about 3/64" so the filaments don't break when installing them.they should be just big enough to get the wire and solder through but not too big or the filament won't stay in place. the LEDs are very fragile and you will break a few.

Use 1/8" black heat shrink to make the single filament in the colon look like a colon.

Wire the entire thing with 26 gage stranded. Get an assortment of colors to help stay organized. 8 colors or more is best.

here's a photo of the clock wiring.

![](https://raw.githubusercontent.com/raleighcopter/big_led_clock/main/clock_bottom.jpg)

here's a photo of the transmitter built on a pro-mini with bme280 sensor and 433MHz transmitter. it can be built using the BME280 or the si7021 sensor. firmware for each is provided. i didn't do a schematic for the transmitter because its fairly trivial. the sensors are i2c and the transmitter pin is listed in the source code. the si7021 doesn't do well with relative humidity above 80% for extended periods which is daily around here so i use the BME280. 

![](https://raw.githubusercontent.com/raleighcopter/big_led_clock/main/transmitter.jpg)

and the transmitter in the gill screen(top off).

![](https://raw.githubusercontent.com/raleighcopter/big_led_clock/main/TX_in_the_bracket.jpg).

Contact me at d.a.taylor.pe@gmail.com
