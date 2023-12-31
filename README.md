
![Alt text](Images/GC9A01_Analogue_Clock_GIF.gif)

# Analogue (Analog) Clock Display using a GC9A01 display attached to an ESP32

## What you will need:
- ESP32 Board
- 1.28" Round LCD Display GC9A01
- Breadboard
- Jumper wires
- Steady power supply of 3.3V/5V
- Accesss to WiFi network
- Arduino IDE

For more information on the GC9A01 display unit, refer to: https://www.waveshare.com/wiki/1.28inch_LCD_Module

Below are basic instructions for configuring your Arduino IDE environment and connecting your display unit to an ESP32 board. It assumes you have
knowledge of the Arduino IDE and use of microprocessor boards. There are many resources on the internet, including YouTube videos if you require
further assistance with these topics

## STEP 1: Set up Arduino IDE to support ESP32 board
1. Open the Arduino IDE
1. Go to  File → Preferences and in the Settings tab, ensure the following URL is added to the 'Additional Boards Managers URLs' field, as follows:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
1. Go to Tools → Board and select 'Boards Manager...'
Search for 'ESP32' and install the software for this board

## STEP 2: Setup Arduino IDE to support GA9A01 libraries
1. This sketch uses the TFT_eSPI Library by Bodmer and thus this library needs to be installed
. Go to Tools → Manage Libraries...
1. Search for 'tft_espi' and install this library

## STEP 3: Configure TFT_eSPI Library
1. Using File Explorer (or similar), go to where your Arduino libraries were installed.
1. Go to the TFT_eSPI folder e.g. C:\Users\your_username\Documents\Arduino\libraries\TFT_eSPI
1. Edit file 'User_Setup.h' and, where all of the drivers are defined, ensure all driver types are commented out except the one for the GA9A01 driver.
1. It is possible to define the ESP32 pins further down in this file but this sketch defines the pin numbers, as detailed below, in C:\Users\your_username\Documents\Arduino\libraries\TFT_eSPI\User_Setups\Setup46_GC9A01_ESP32.h
1. Edit file 'User_Setup_Select.h' and ensure the line with '#include <User_Setups/Setup46_GC9A01_ESP32.h>' is not commented out i.e. it is enabled.

## STEP 4: Configure NTP_Time.h File for local Timezone
1. Download the files in this project.
1. Edit NTP_Time.h, specifying your local timezone (around line #256), based on the Timezone reference definitions in the Settings section.

## STEP 5: Customise GC9A01A_Clock_ESP32_Sprite_GitHub.ino
1. Edit this file and enter your WiFi credentials (name and password)

## STEP 6: Connecting GA9A01 display to ESP32
Below is how I connected the display unit to the ESP32 although other mappings would be valid, noting that some GPIO pins have restrictions:

| TFT_eSPI | ESP32 | GC9A01 | Other Terms |
| -------- | ----- | ------ | ----------- |
|          |  3V3  |  VCC   |             |
|          |  GND  |  GND   |             |
|   MOSI   |  GP23 |  DIN   |   SDA       |
|   SCLK   |  GP18 |  CLK   |             |
|   CS     |  GP15 |  CS    |             |
|   DC     |  GP2  |  DC    |             |
|   RST    |  GP4  |  RST   |             |

