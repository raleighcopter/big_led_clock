#include "Adafruit_Si7021.h"
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
Adafruit_Si7021 sensor = Adafruit_Si7021();

RH_ASK driver(2000, 0, 2, 0);
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS


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
data[0] = (sensor.readHumidity() + .5),2;
data[1] = (1.8 * (sensor.readTemperature() + 0.5) +32),2;
    driver.send((uint8_t*)data,4);
    driver.waitPacketSent();
    Serial.print("Sent: ");Serial.print(data[0]);Serial.println(data[1]);
    delay(10000);
}
