//transmits temp and relative humidity data. 
//build on arduino nano or pro mini.
//uses an bme280 sensor and cheap 433 MHz transmitter.
//the transmitter connects to D2. 
//sends a new set of data about every 3 seconds.

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#endif
Adafruit_BME280 bme; // I2C

RH_ASK driver(2000, 0, 2, 0);
// RH_ASK driver(2000, 4, 5, 0); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS


void setup()
{
unsigned status;

status = bme.begin(0x76);

#ifdef RH_HAVE_SERIAL
    Serial.begin(9600);	  // Debugging only
#endif
    if (!driver.init())
#ifdef RH_HAVE_SERIAL
         Serial.println("init failed");
#else
         Serial.println("init success");	;
#endif

}
void loop() {
//  Serial.print("Humidity:    ");
//  Serial.print(bme.readHumidity(), 2);
//  Serial.print("\tTe0mperature: ");
//  Serial.println(bme.readTemperature(), 2);
int data[2];
data[0] = int (bme.readHumidity()),2;
data[1] = int (1.8 * (bme.readTemperature()) +32),2;
    driver.send((uint8_t*)data,4);
    driver.waitPacketSent();
  //  Serial.print("Sent: ");Serial.print(data[0]);Serial.println(data[1]);
    delay(3000);
}
