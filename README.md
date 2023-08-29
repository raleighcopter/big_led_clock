Big Clock

![](https://raw.githubusercontent.com/raleighcopter/big_led_clock/main/clock.jpg)

Some notes...

Order extra LED filaments. They're brittle, fragile and you will break more than one. Sourced from AliExpress.

THE 433 MHZ functionality is now working and an optional transmitter with si7021 temp/rh sensor can be used to wirelessly transmit weather data to the clock. you can build the clock with the receiver and build the transmitter at a later date and the clock will only display time.

The 3v converter can be omitted but you won't be able to set the brightness of the LEDs if you do so play with the value of the series resistors for the LEDs if you omit the buck converter. You want at least 100 ohm series resistors, possibly higher.
  
You can omit the DST switch and just use the hour offset pot to adjust the hour for DST.

I used 1/4" poplar for the base and painted it with some hammer tone grey paint.

I used 3er perf board to build the circuit. It's available cheapest on eBay.

The GPS module is a neo6m module from AliExpress and it provides an accurate time source.

Drill the holes for the filament wires out to about 3/64" so the filaments don't break when installing them.they should be just big enough to get the wire and solder through but not too big or the filament won't stay in place. the LEDs are very fragile and you will break a few.

Use heat shrink to make the single filament in the colon look like a colon.

Wire the entire thing with 22 gage stranded. Get an assortment of colors to help stay organized. 8 colors or more is best.

here's a p[hoto of the clock wiring.

![](https://raw.githubusercontent.com/raleighcopter/big_led_clock/main/clock_bottom.jpg)

here's a photo of the transmitter built on a pro-mini with bme280 sensor and 433MHz transmitter.

![](https://raw.githubusercontent.com/raleighcopter/big_led_clock/main/transmitter.jpg)

