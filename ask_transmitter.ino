//transmits temp and relative humidity data. 
//build on arduino nano or pro mini.
//uses an si7021 sensor and cheap 433 MHz transmitter.
//remove the power LED and the green LED from teh si7021
//the transmitter connects to D2. 
//sends a new set of data about every second.

#include "Adafruit_Si7021.h"
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
Adafruit_Si7021 sensor = Adafruit_Si7021();

RH_ASK driver(2000, 0, 2, 0);

void setup()
{
#ifdef RH_HAVE_SERIAL
    Serial.begin(9600);	  // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
         Serial.println("init success");	;
#endif
  Serial.println("Si7021 test!");
  
  if (!sensor.begin()) {
    Serial.println("Did not find Si7021 sensor!");
    while (true)
      ;
  }
}
void loop() {
  Serial.print("Humidity:    ");
  Serial.print(sensor.readHumidity(), 2);
  Serial.print("\tTemperature: ");
  Serial.println(sensor.readTemperature(), 2);
int data[2];
data[0] = (sensor.readHumidity()),2;
data[1] = (1.8 * sensor.readTemperature() +32),2;
    driver.send((uint8_t*)data,4);
    driver.waitPacketSent();
    Serial.print("Sent: ");Serial.print(data[0]);Serial.println(data[1]);
    delay(1000);
}
