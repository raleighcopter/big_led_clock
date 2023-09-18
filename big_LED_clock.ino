/*


the segments of the digits are labeled as follows
         _____    
        /  A /   
       /F   /B  
      /____/     
     /  G /    
    /E   /C  
   /____/     
     D           

the gps is wired to serial3 (14,15)

*/


//make config changes in the area of code below

int leading_zero_blanking = 1; //set to 1 to enable
int format_12 = 1; //set to 1 to enable 12 hour time
int receiver_code = 152; //must match transmitter code
int dimmable_display = 1; //set to 1 if the display is wired with a PNP transistor (ss8550 with 1k base resistor) on the display power wire
int nite_off = 23; // dim display at this hour and after
int morning_on = 7; // dim display before this hour
int dim_level = 5; //amount to dim display (0-255)
int display_dim_pin = 44; //optional pwm pin for display digit dimming
int offset_pin = A15; //analog pin for hour offset pot
int dst_pin = 48; //the dst button pin
int RF_data_pin = 40; //the receiver data pin

//pins that the display segments are wired to are defined below

int hours_ten_a = 27;
int hours_ten_b = 28;
int hours_ten_c = 29;
int hours_ten_d = 30;
int hours_ten_e = 31;
int hours_ten_f = 46;
int hours_ten_g = 47;

int hours_one_a = 20;
int hours_one_b = 21;
int hours_one_c = 22;
int hours_one_d = 23;
int hours_one_e = 24;
int hours_one_f = 25;
int hours_one_g = 26;

int minutes_ten_a = 9;
int minutes_ten_b = 10;
int minutes_ten_c = 11;
int minutes_ten_d = 16;
int minutes_ten_e = 17;
int minutes_ten_f = 18;
int minutes_ten_g = 19;

int minutes_one_a = 2;
int minutes_one_b = 7;
int minutes_one_c = 6;
int minutes_one_d = 5;
int minutes_one_e = 4;
int minutes_one_f = 3;
int minutes_one_g = 8;

int colon = 13; // the pin that the colon is wired to. must be a PWM capable pin

// make config changes above this line only

#include <TinyGPS++.h>  //TinyGPSPlus library
#include <RH_ASK.h>     //Radiohead library

RH_ASK driver(2000, RF_data_pin, 0, 0);

static const uint32_t GPSBaud = 9600;

int on = 1;
int off = 0;
int dot_count;
int colon_val;
int hi_dot = 255;

int hour_offset;

int a;
int b;
int c;
int d;
int e;
int f;
int g;

int hours = 0;
int minutes = 0;
int seconds = 0;
int hours_ten = 0;
int hours_one = 0;
int minutes_ten = 0;
int minutes_one = 0;

int weather_time = 0;
int weather_valid = 0;
int Temp;
int RH;

TinyGPSPlus gps;

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

  Serial3.begin(GPSBaud);

  pinMode(hours_ten_a, OUTPUT);
  pinMode(hours_ten_b, OUTPUT);
  pinMode(hours_ten_c, OUTPUT);
  pinMode(hours_ten_d, OUTPUT);
  pinMode(hours_ten_e, OUTPUT);
  pinMode(hours_ten_f, OUTPUT);
  pinMode(hours_ten_g, OUTPUT);
  pinMode(hours_one_a, OUTPUT);
  pinMode(hours_one_b, OUTPUT);
  pinMode(hours_one_c, OUTPUT);
  pinMode(hours_one_d, OUTPUT);
  pinMode(hours_one_e, OUTPUT);
  pinMode(hours_one_f, OUTPUT);
  pinMode(hours_one_g, OUTPUT);
  pinMode(minutes_ten_a, OUTPUT);
  pinMode(minutes_ten_b, OUTPUT);
  pinMode(minutes_ten_c, OUTPUT);
  pinMode(minutes_ten_d, OUTPUT);
  pinMode(minutes_ten_e, OUTPUT);
  pinMode(minutes_ten_f, OUTPUT); 
  pinMode(minutes_ten_g, OUTPUT);
  pinMode(minutes_one_a, OUTPUT);
  pinMode(minutes_one_b, OUTPUT);
  pinMode(minutes_one_c, OUTPUT);
  pinMode(minutes_one_d, OUTPUT);
  pinMode(minutes_one_e, OUTPUT);
  pinMode(minutes_one_f, OUTPUT);
  pinMode(minutes_one_g, OUTPUT);
  pinMode(dst_pin, INPUT); 
}

void loop()
{
//get weather data if available
  int data[3] = {0};
      uint8_t buflen = sizeof(data);
    if (driver.recv((uint8_t*)data, &buflen));
   {
  if (data[0] == receiver_code) {  //throw away bad data
    RH = data[1];
    Temp = data[2];
    weather_time =  seconds; //record time of weather aquisition
    weather_valid = 1; //mark weather as valid
  }
}


  // This sketch displays information every time a new NMEA sentence is correctly encoded.
  while (Serial3.available() > 0)
    if (gps.encode(Serial3.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  hour_offset = map(analogRead(offset_pin), 0, 1023, 23, 0) + digitalRead(dst_pin);

}

void displayInfo()
{
  if (gps.time.isValid())
// store values in working variables 
  {
  hours = gps.time.hour();
  minutes = gps.time.minute();
  seconds = gps.time.second();

  hours = hours + hour_offset;



//invalidate weather data that's too old
if (seconds - weather_time >= 10) {
  weather_valid = 0;
}

//adjust to 24 hour local time
  if (hours > 24) hours = hours - 24;
  if (hours < 1) hours = hours + 24;

// dim during off hours
    if ((hours >= nite_off) or (hours < morning_on)) {
      hi_dot = dim_level; //dim the colon
      if (dimmable_display == 1) { //dim the digits if they're dimmable
        analogWrite(display_dim_pin, (255 - dim_level));
      }
      else {
        on = 0; //digits not dimmable so turn them off
      }
    } else {
      hi_dot = 255;
      on = 1;
      analogWrite(display_dim_pin, 0);
    }

//convert to 12 hour format if format_12 = 1  
  if (format_12 == 1) {
    if (hours > 12) hours = hours - 12;
    if (hours < 1) hours = hours + 12;
    }
   }


//determine if we should display time or temp
if ((weather_valid == 1) and seconds % 15 >= 13) {
//display RH
    colon_val = off;
    hours_ten = (RH/10);
    hours_one = (RH - (10 * hours_ten));
    minutes_ten = 11;
    minutes_one = 12;
}

if ((weather_valid == 1) and (seconds % 15 >= 11 and seconds %15 < 13) ) {
  colon_val = off;
//display temp
  if ((Temp < 0) and (Temp >= -9)) {
    Temp = abs(Temp);
    hours_ten = 10;  // minus
    hours_one = Temp;
    minutes_ten = 10; //degrees
    minutes_one = 11; //F
  } else if (Temp <= -10) {
    Temp = abs(Temp);
    hours_ten = 10; //minus
    hours_one = (Temp/10);
    minutes_ten = (Temp - (10 * hours_one));
    minutes_one = 10; //degrees
  } else if ((Temp >= 0) and (Temp <100)) {
    hours_ten = (Temp/10);
    hours_one = (Temp - (10 * hours_ten));
    minutes_ten = 10; //degrees
    minutes_one = 11; //F
  } else if (Temp >= 100) {
    hours_ten = 1;
    Temp = Temp - 100;
    hours_one = (Temp/10);
    minutes_ten = (Temp - (10 * hours_one));
    minutes_one = 10; //degrees     
    }
}

if ((seconds % 15 <= 10) or (weather_valid == 0)) {
//display time
// calculate values for separate digits to display time
  hours_ten = (hours/10);
  hours_one = (hours - (10 * hours_ten));
  minutes_ten = (minutes/10);
  minutes_one = (minutes - (10 * minutes_ten));

//do colon stuff....
  dot_count = seconds % 2;

  switch(dot_count)
  {
    case 0: colon_val = hi_dot; break;
    case 1: colon_val = off; break; 
  }
}


//do display stuff
// write colon

analogWrite(colon, colon_val);

// set the display segments

switch(hours_ten)
{
  case 0: if (leading_zero_blanking = 0) {
    a=on;b=on;c=on;d=on;e=on;f=on;g=off;
    } else { 
    a=off;b=off;c=off;d=off;e=off;f=off;g=off;
    }
    break;
  case 1: a=off;b=on;c=on;d=off;e=off;f=off;g=off;break;
  case 2: a=on;b=on;c=off;d=on;e=on;f=off;g=on;break;
  case 3: a=on;b=on;c=on;d=on;e=off;f=off;g=on;break;
  case 4: a=off;b=on;c=on;d=off;e=off;f=on;g=on;break;
  case 5: a=on;b=off;c=on;d=on;e=off;f=on;g=on;break;
  case 6: a=on;b=off;c=on;d=on;e=on;f=on;g=on;break;
  case 7: a=on;b=on;c=on;d=off;e=off;f=off;g=off;break;
  case 8: a=on;b=on;c=on;d=on;e=on;f=on;g=on;break;
  case 9: a=on;b=on;c=on;d=off;e=off;f=on;g=on;break;
  case 10: a=off;b=off;c=off;d=off;e=off;f=off;g=on;break; // - symbol    
}
// write hours_ten digit display pins
  
digitalWrite(hours_ten_a, a);
digitalWrite(hours_ten_b, b);
digitalWrite(hours_ten_c, c);
digitalWrite(hours_ten_d, d);
digitalWrite(hours_ten_e, e);
digitalWrite(hours_ten_f, f);
digitalWrite(hours_ten_g, g);

switch(hours_one)
{
  case 0: a=on;b=on;c=on;d=on;e=on;f=on;g=off;break;
  case 1: a=off;b=on;c=on;d=off;e=off;f=off;g=off;break;
  case 2: a=on;b=on;c=off;d=on;e=on;f=off;g=on;break;
  case 3: a=on;b=on;c=on;d=on;e=off;f=off;g=on;break;
  case 4: a=off;b=on;c=on;d=off;e=off;f=on;g=on;break;
  case 5: a=on;b=off;c=on;d=on;e=off;f=on;g=on;break;
  case 6: a=on;b=off;c=on;d=on;e=on;f=on;g=on;break;
  case 7: a=on;b=on;c=on;d=off;e=off;f=off;g=off;break;
  case 8: a=on;b=on;c=on;d=on;e=on;f=on;g=on;break;
  case 9: a=on;b=on;c=on;d=off;e=off;f=on;g=on;break;           
}
// write hours_one digit display pins
  
digitalWrite(hours_one_a, a);
digitalWrite(hours_one_b, b);
digitalWrite(hours_one_c, c);
digitalWrite(hours_one_d, d);
digitalWrite(hours_one_e, e);
digitalWrite(hours_one_f, f);
digitalWrite(hours_one_g, g);

switch(minutes_ten)
{
  case 0: a=on;b=on;c=on;d=on;e=on;f=on;g=off;break;
  case 1: a=off;b=on;c=on;d=off;e=off;f=off;g=off;break;
  case 2: a=on;b=on;c=off;d=on;e=on;f=off;g=on;break;
  case 3: a=on;b=on;c=on;d=on;e=off;f=off;g=on;break;
  case 4: a=off;b=on;c=on;d=off;e=off;f=on;g=on;break;
  case 5: a=on;b=off;c=on;d=on;e=off;f=on;g=on;break;
  case 6: a=on;b=off;c=on;d=on;e=on;f=on;g=on;break;
  case 7: a=on;b=on;c=on;d=off;e=off;f=off;g=off;break;
  case 8: a=on;b=on;c=on;d=on;e=on;f=on;g=on;break;
  case 9: a=on;b=on;c=on;d=off;e=off;f=on;g=on;break;
  case 10: a=on;b=on;c=off;d=off;e=off;f=on;g=on;break; // degree symbol
  case 11: a=off;b=off;c=off;d=off;e=on;f=off;g=on;break; //r      
}
// write minutes_ten digit display pins
  
digitalWrite(minutes_ten_a, a);
digitalWrite(minutes_ten_b, b);
digitalWrite(minutes_ten_c, c);
digitalWrite(minutes_ten_d, d);
digitalWrite(minutes_ten_e, e);
digitalWrite(minutes_ten_f, f);
digitalWrite(minutes_ten_g, g);

switch(minutes_one)
{
  case 0: a=on;b=on;c=on;d=on;e=on;f=on;g=off;break;
  case 1: a=off;b=on;c=on;d=off;e=off;f=off;g=off;break;
  case 2: a=on;b=on;c=off;d=on;e=on;f=off;g=on;break;
  case 3: a=on;b=on;c=on;d=on;e=off;f=off;g=on;break;
  case 4: a=off;b=on;c=on;d=off;e=off;f=on;g=on;break;
  case 5: a=on;b=off;c=on;d=on;e=off;f=on;g=on;break;
  case 6: a=on;b=off;c=on;d=on;e=on;f=on;g=on;break;
  case 7: a=on;b=on;c=on;d=off;e=off;f=off;g=off;break;
  case 8: a=on;b=on;c=on;d=on;e=on;f=on;g=on;break;
  case 9: a=on;b=on;c=on;d=off;e=off;f=on;g=on;break;
  case 10: a=on;b=on;c=off;d=off;e=off;f=on;g=on;break; // degree symbol
  case 11: a=on;b=off;c=off;d=off;e=on;f=on;g=on;break; // F
  case 12: a=off;b=off;c=on;d=off;e=on;f=on;g=on;break; //h           
}
// write minutes_one digit display pins
  
digitalWrite(minutes_one_a, a);
digitalWrite(minutes_one_b, b);
digitalWrite(minutes_one_c, c);
digitalWrite(minutes_one_d, d);
digitalWrite(minutes_one_e, e);
digitalWrite(minutes_one_f, f);
digitalWrite(minutes_one_g, g);
  
}
