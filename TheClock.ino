#include <RTClib.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUM_LEDS 8
#define PIN 9
RTC_DS3231 rtc;
Adafruit_7segment matrix = Adafruit_7segment();
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

int hour;
int minute;
int second;
String s_hour;
String s_minute;
String s_nowTime;
int nowTime;
bool ColonOn;

void setup() {
  // starts the LEDs
  strip.begin();
  strip.show(); 
  //starts I2C 
  Wire.begin();
  //starts the clock display
  matrix.begin(0x70);
  //sets the time on the clock
  if (rtc.lostPower()){
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }  
}

void loop() {
  DateTime now = rtc.now();

  hour = now.hour();
  if (hour == 0){
    hour = hour + 1;  
  }
  if (hour > 12){
    hour = hour - 12;
  }
  minute = now.minute();
  s_hour = String(hour);
  s_minute = String (minute, DEC);
  if (minute <= 9){
    s_minute = "0" + s_minute;
  }
  s_nowTime = s_hour + s_minute;
  nowTime = s_nowTime.toInt();
  matrix.print(nowTime, DEC);
  if (ColonOn == false){
    matrix.drawColon(true);
    ColonOn = true;
  }
  else{
    matrix.drawColon(false);
    ColonOn = false;
  }
  matrix.writeDisplay(); 
  
  colorWipe(strip.Color(255, 0, 0), 10);
  delay(1000);
  
  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
