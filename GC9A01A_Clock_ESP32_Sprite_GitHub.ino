/*

This sketch draws an analogue clock face (BigBen) and moves clock hands to display the correct time.

ESP32 Note: When uploading a sketch in Arduino IDE, may need to hold the BOOT button for 1-2 secs when the
message "Connecting....." appears and/or disconnect display from microprocessor when uploading the sketch.
Use the 'ESP32 Dev Board' setting.

Based on the following sketches:
  1. YouTube video 'Using Images and Sprites to Make a Gauge | Beginner How To Converting Images to CPP to Sprite'
   https://www.youtube.com/watch?v=wbDrjoxb51I. This video provides details on how to create images and push sprites.
  2. C:\Users\your_username\Documents\Arduino\libraries\TFT_eSPI-master\examples\Smooth Graphics\Anti-aliased_Clock.
   This sketch provides the code to get NTP Time.


In Libraries\TFT_eSPI\User_Setup.h, enable GC9A01_DRIVER.
In Libraries\TFT_eSPI\User_Setup_Select.h, enable <User_Setups/Setup46_GC9A01_ESP32.h>

This is the setup used by this sketch as defined in libraries\TFT_eSPI\User_Setups\Setup46_GC9A01_ESP32.h

#define USER_SETUP_ID 46

#define GC9A01_DRIVER

#define TFT_MISO 19   // not used
#define TFT_MOSI 23   // DIN/SDA (data in)
#define TFT_SCLK 18   // CLK
#define TFT_CS    15  // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST   4  // Reset pin (could connect to RST pin)

Below are the various functions that are called:

****************************************************************************************
** Function name:           setColorDepth (from sprite.cpp in 'Arduino\libraries\TFT_eSPI\Extensions\Sprite.cpp')
** Description:             Set bits per pixel for colour (1, 8 or 16)

setColorDepth(int8_t b)
b = bits per pixel
****************************************************************************************
** Function name:           pushImage (from sprite.cpp in 'Arduino\libraries\TFT_eSPI\Extensions\Sprite.cpp'
** Description:             push 565 colour FLASH (PROGMEM) image into a defined area

pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data)
x = X coordinate
y = Y coordinate
w = width of image
h = height of image
*data = where image is stored as code
****************************************************************************************
** Function name:           createSprite (from sprite.cpp in 'Arduino\libraries\TFT_eSPI\Extensions\Sprite.cpp'
** Description:             Create a sprite (bitmap) of defined width and height

createSprite(int16_t w, int16_t h)
w = width of image
h = height of image
****************************************************************************************
** Function name:           setPivot from (from 'Arduino\libraries\TFT_eSPI\TFT_eSPI.cpp')
** Description:             Set the pivot point on the TFT

setPivot(int16_t x, int16_t y)
x = X coordinate
y = Y coordinate
****************************************************************************************

*/

#define WIFI_SSID      "Your_SSID"
#define WIFI_PASSWORD  "Your_Password"

#include <Arduino.h>
#include "SPI.h"
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

#include <time.h>
#include <WiFi.h>
#include <Timezone.h>
#include <TimeLib.h>
#include "NTP_Time.h"

#include "BigBen_16bit.h"
#include "hourHand.h"
#include "minuteHand.h"
#include "secondHand.h"

int sDeg,mDeg,hDeg;

unsigned long ms = millis();
unsigned long mms = millis();
byte start = 1;         // initial start flag
byte display_sHand = 1; // dsplay seconds hand 0=no, 1=yes
byte hh, mm,ss;

TFT_eSprite clock_Face = TFT_eSprite(&tft);   // Declare Sprite object "spr" with pointer to "tft" object
TFT_eSprite second_Hand = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object
TFT_eSprite minute_Hand = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object
TFT_eSprite hour_Hand = TFT_eSprite(&tft);    // Declare Sprite object "spr" with pointer to "tft" object
// ----------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  Serial.println("GC9A01A Clock Face");
  
// Obtain current time and set variables for the Second, Minute and Hour
  syncTime();                     // Obtain current internet time using NTP_Time.h routines
  t = now();                      // returns the current time as seconds since Jan 1 1970
  hh = hour(t);                   // the hour now  (0-23)
  mm = minute(t);                 // the minute now (0-59)
  ss = second(t);                 // the second now (0-59)

// initial setup of display unit
  tft.init();                     // initialise the display unit
  tft.setRotation(2);             // 0 = 0 degress, 2 = 180 degrees
  tft.fillScreen(TFT_WHITE);      // fill screen with white background
  
// swap byte order when rendering to correct different image endianness 
  second_Hand.setSwapBytes(true);
  minute_Hand.setSwapBytes(true);
  hour_Hand.setSwapBytes(true);
  
// Create images for the Second, Minute and Hour hands
  createSecondHand();
  createMinuteHand();
  createHourHand();

}

void loop() {

// Only update the screen once every second and at the start
  if(millis()-ms>=1000 || start) {
    ms = millis();
    if(++ss>59) {
      ss=0;
      if(++mm>59) {
        mm=0;
        if(++hh>23) hh=0;
      }
    }
// Set the angles for the Second, Minute and Hour hands
    sDeg = ss*6;
    if(ss==0 || start) {
      start = 0;
      if(ss==0) mms = millis();
      mDeg = mm*6+sDeg/60;
      hDeg = hh*30+mDeg/12;
      }
  
    clockUpdate(sDeg,mDeg,hDeg); // call routine to push the sprites for the clock face and clock hands
  }
}

// Create Seconds hand sprite and push the image of the Seconds hand to that sprite
void createSecondHand() {
  second_Hand.setColorDepth(8); // set for 8 bits per pixel
  second_Hand.createSprite(10,136);
  second_Hand.pushImage(0,0,10,136,secondHand);
  second_Hand.setPivot(5,102);
}

// Create Minutes hand sprite and push the image of the Minutes hand to that sprite
void createMinuteHand() {
  minute_Hand.setColorDepth(8);
  minute_Hand.createSprite(16,100);
  minute_Hand.pushImage(0,0,16,100,minuteHand);
  minute_Hand.setPivot(8,79);
}
// Create Hour hand sprite and push the image of the Hour hand to that sprite
void createHourHand() {
  hour_Hand.setColorDepth(8);
  hour_Hand.createSprite(22,76);
  hour_Hand.pushImage(0,0,22,76,hourHand);
  hour_Hand.setPivot(10,51);
}

// Create the background sprite i.e. the clock face and push the image of the clock face to that sprite
void createBackground() {
  clock_Face.setColorDepth(8);
  clock_Face.createSprite(240,240);
  clock_Face.setPivot(120,120);
  tft.setPivot(120,120);
  clock_Face.fillSprite(TFT_TRANSPARENT);
  clock_Face.pushImage(0,0,240,240,BigBen_16bit);
}

// On each update, re-create the clock face, and push the sprites of the Seconds, Minutes and Hour hands to their new positions on the display 
void clockUpdate(int16_t angle_secondHand, int16_t angle_minuteHand, int16_t angle_hourHand) {
  createBackground();
  if (display_sHand) second_Hand.pushRotated(&clock_Face, angle_secondHand, TFT_TRANSPARENT); // Check if Seconds hand is to be displayed
  minute_Hand.pushRotated(&clock_Face, angle_minuteHand, TFT_TRANSPARENT);
  hour_Hand.pushRotated(&clock_Face, angle_hourHand, TFT_TRANSPARENT);
  clock_Face.pushSprite(0,0,TFT_TRANSPARENT);
}

