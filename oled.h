// Include the correct display library
// https://github.com/squix78/esp8266-oled-ssd1306
// For a connection via I2C using Wire include
//#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

// Include the UI lib
#include "OLEDDisplayUi.h"

// Include custom images
#include "images.h"

// Use the corresponding display class:
// Initialize the OLED display using Wire library
SSD1306  display(0x3c, D3, D4); //GPIO0, GPIO2 

OLEDDisplayUi ui ( &display );

int screenW = 128;
int screenH = 64;
int clockCenterX = screenW / 2;
int clockCenterY = ((screenH - 16) / 2) + 16; // top yellow part is 16 px height
int clockRadius = 23;



// utility function for digital clock display: prints leading 0
String twoDigits(int digits) {
  if (digits < 10) {
    String i = '0' + String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

void dis_stat_prog(String stat) {

  display.clear();
  // Print to the screen
  display.println(stat);
  // Draw it to the internal screen buffer
  display.drawLogBuffer(0, 0);
  display.drawProgressBar(0, 60, 128, 10, gv_progress);
  // Display it on the screen
  display.display();

}

void dis_stat_prog(String stat, uint8_t progress) {

  gv_progress = progress;
  dis_stat_prog( stat );
}
