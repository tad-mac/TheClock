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
#define LeftButtonPin 3
#define LeftButtonLEDPin 4
#define RightButtonPin 5
#define RightButtonLEDPin 6 

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
bool ColonOn = true;
int RightLEDState = HIGH;
int RightButtonState;
int LastRightButtonState = LOW;
int LeftLEDState;
int LeftButtonState;
int LastLeftButtonState = LOW;
unsigned long ColonFlashTime = 1000;
unsigned long LastColonFlashTime = 0;
unsigned long RightLastDebounceTime = 0;
unsigned long RightDebounceDelay = 50;
unsigned long LeftLastDebounceTime = 0;
unsigned long LeftDebounceDelay = 50;

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
  //intialize the pins for the Pushbuttons and Pushbutton LEDs
  pinMode(RightButtonPin, INPUT);
  pinMode(LeftButtonPin, INPUT);
  pinMode(RightButtonLEDPin, OUTPUT);
  pinMode(LeftButtonLEDPin, OUTPUT);
  digitalWrite(RightButtonLEDPin, RightLEDState);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Button Debouncing
  int RightButtonReading = digitalRead(RightButtonPin);
  int LeftButtonReading = digitalRead(LeftButtonPin);
  
  if (RightButtonReading != LastRightButtonState){
    RightLastDebounceTime = millis();
  }

  if (LeftButtonReading != LastLeftButtonState){
    LeftLastDebounceTime = millis();
  }

  if ((millis() - RightLastDebounceTime) > RightDebounceDelay){
    if (RightButtonReading != RightButtonState){
      RightButtonState = RightButtonReading;
      if (RightButtonState == HIGH)
        RightLEDState = !RightLEDState;
    }
  }

  if ((millis() - LeftLastDebounceTime) > LeftDebounceDelay){
    if (LeftButtonReading != LeftButtonState){
      LeftButtonState = LeftButtonReading;
      if (LeftButtonState == HIGH)
        LeftLEDState = !LeftLEDState;
    }
  }
  
  digitalWrite(RightButtonLEDPin, RightLEDState);
  LastRightButtonState = RightButtonReading;

  digitalWrite(LeftButtonLEDPin, LeftLEDState);
  LastLeftButtonState = LeftButtonReading;
  
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
  
  //This blinks the Colon in the middle of the Clock every second

  LastColonFlashTime = millis();
  if ((millis() - LastColonFlashTime) > ColonFlashTime){
    ColonOn != ColonOn;
  }
  matrix.drawColon(ColonOn);
  matrix.writeDisplay(); 
  
  //colorWipe(strip.Color(255, 0, 0), 10);
  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
