/*


the segments of the digits are labeled as follows
         _____    
        /  A /   
       /F   /B  
      /____/     
     / G  /    
    /E   /C  
   /____/     
     D           

the pot is wired to A15, gnd, 5v
the gps is wired to serial3 (14,15)
the DST switch is wired to D48
the RF Receiver is wired to D40

*/
#include <TinyGPS++.h>
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif
RH_ASK driver(2000, 40, 0, 0);

static const uint32_t GPSBaud = 9600;
int on = 1;
int off = 0;

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

int dot_count;
int colon = 13;
int colon_val;
int hi_dot = 255;
int lo_dot = 0;
int offset_pin = A15;
int dst_pin = 48;
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
int nite_off = 22;  //dim after this hour. set to 24 to disable
int morning_on = 7; //dim before this hour. set to 0 to disable
int dim_level = 5;  // the brightness of the colon during dim hours
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
  int data[2] = {0};
      uint8_t buflen = sizeof(data);
    if (driver.recv((uint8_t*)data, &buflen));
   {
  if (data[0] != 0) {  //throw away bad data
    RH = data[0];
    Temp = data[1];
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
  hour_offset = map(analogRead(offset_pin), 0, 1023, -11, 0) + digitalRead(dst_pin);

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
//weather_valid = 0;
//adjust to 24 hour local time
  if (hours > 24) hours = hours - 24;
  if (hours < 1) hours = hours + 24;

// dim during off hours
    if ((hours > nite_off) or (hours < morning_on)) {
    on = 0; 
    hi_dot = dim_level;
    }
    else {
      on = 1;
      hi_dot = 255;
    }

//convert to 12 hour format  
  if (hours > 12) hours = hours - 12;
  if (hours < 1) hours = hours + 12;
   }


//determine if we should display time or temp
if ((weather_valid == 1) and seconds % 15 >= 13) {
//display RH
    colon_val = lo_dot;
    hours_ten = (RH/10);
    hours_one = (RH - (10 * hours_ten));
    minutes_ten = 11;
    minutes_one = 12;
}

if ((weather_valid == 1) and (seconds % 15 >= 11 and seconds %15 < 13) ) {
  colon_val = lo_dot;
//display temp
  if (Temp < 0) {
    Temp = abs(Temp);
    hours_ten = 10;
    hours_one = (Temp/10);
    minutes_ten = (Temp - (10 * hours_one));
    minutes_one = 10;
  } else if ((Temp >=0) and (Temp <100)) {
    hours_ten = (Temp/10);
    hours_one = (Temp - (10 * hours_ten));
    minutes_ten = 10;
    minutes_one = 11;
  } else if (Temp >99) {
    hours_ten = 1;
    Temp = Temp - 100;
    hours_one = (Temp/10);
    minutes_ten = (Temp - (10 * hours_one));
    minutes_one = 10;      
    }
}

if ((seconds % 15 <= 10) or (weather_valid == 0))  {
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
    case 1: colon_val = lo_dot; break; 
  }
}


//do display stuff
// write colon

analogWrite(colon, colon_val);

// set the display segments

switch(hours_ten)
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
