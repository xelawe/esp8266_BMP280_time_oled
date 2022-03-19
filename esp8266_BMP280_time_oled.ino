/*
Ersetzt durch Tasmota mit Oled
Template:
{"NAME":"time_oled","GPIO":[1,1,576,33,32,640,1,1,417,418,608,416,1,4768],"FLAG":0,"BASE":18}

Displaymode 0
DisplayRotate 2
DisplayWidth 128
DisplayHeight 64
Poweronstate 0

Rules:
rule1 on System#Boot do displaytext [zs2x-10] %Mem1% [s2y25x-10] %Mem2% [s1ty50x0] [Ty50x45] endon
      on Time#Minute do displaytext [zs2x-10] %Mem1% [s2y25x-10] %Mem2% [s1ty50x0] [Ty50x45] endon
      on tele-BME280#Temperature do Mem1 %value% C endon  
      on tele-BME280#Humidity do Mem2 %value% % endon
      on ANALOG#Illuminance<1 do power2 off endon
      on ANALOG#Illuminance>5 do power2 on endon
rule1 1

      
*/

/**
   The MIT License (MIT)

   Copyright (c) 2016 by Daniel Eichhorn
   Copyright (c) 2016 by Fabrice Weinberg

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

*/
//#include <cy_serdebug.h>
#include <cy_serial.h>

const char *gc_hostname = "espbmeoled";

#include <Metro.h>

#define btnpin 4
#define ledpinbl 13
#define ledpinrt 15
#define ledpingn 12
#define LDRPin (A0)
int LDRValue;
char gv_LDRThres_c[3];
int gv_LDRThres = 30;

char gv_sensnbr_t_c[3];
char gv_sensnbr_h_c[3];
int gv_sens_nbr_t = 19;
int gv_sens_nbr_h = 20;

uint8_t gv_progress;

const int CMD_WAIT = 0;
const int CMD_BUTTON_CHANGE = 1;
int cmd = CMD_WAIT;
int buttonState = HIGH;
static long startPress = 0;

#include "cy_wifi.h"
#include "cy_ota.h"

#include "bme280_tool.h"
#include "oled.h"
#include "weather.h"
#include "ntptool.h"
#include "oled_frames.h"


Metro go_metro = Metro(30000);

void restart() {
  ESP.reset();
  delay(1000);
}

void reset() {
  //reset wifi credentials
  WiFi.disconnect();
  delay(1000);
  ESP.reset();
  delay(1000);
}


ICACHE_RAM_ATTR void IntBtn() {
  cmd = CMD_BUTTON_CHANGE;
}

void setup() {
  cy_serial::start(__FILE__);

  display.init();
  display.setContrast(255);
  // Initialize the log buffer
  // allocate memory to store 4 lines of text and 30 chars per line.
  display.setLogBuffer(4, 30);

  dis_stat_prog(__FILE__, 0);
  delay(3000);

  dis_stat_prog("Setup started", 0);

  //init_wifi();
  dis_stat_prog("WiFi: wait to connect", 20);
  wifi_init(gc_hostname);
  dis_stat_prog("connected...yeey", 60);

  init_ota(gv_clientname);

  //setup button
  pinMode(btnpin, INPUT);
  attachInterrupt(btnpin, IntBtn, CHANGE);

  init_time();

  init_bme280();
  do_sensor();

  dis_stat_prog("done", 100);

  delay(500);

  init_oled();
  // The ESP is capable of rendering 60fps in 80Mhz mode
  // but that won't give you much time for anything else
  // run it in 160Mhz mode or just set it to 30 fps
  ui.setTargetFPS(60);

  // Customize the active and inactive symbol
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);

  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(TOP);

  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);

  // Add frames
  ui.setFrames(frames, frameCount);
  //ui.disableAutoTransition();

  // Add overlays
  ui.setOverlays(overlays, overlaysCount);

  // Initialising the UI will init the display too.
  ui.init();

  //display.flipScreenVertically();
  ui.update();

}


void loop() {
  int remainingTimeBudget;

  LDRValue = analogRead(LDRPin);

  if ( LDRValue < gv_LDRThres ) {
    remainingTimeBudget = 1;
    display.displayOff();
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
  }
  else {
    digitalWrite(2, LOW);
    pinMode(2, INPUT_PULLUP);
    remainingTimeBudget = ui.update();
    display.displayOn();
  }


  switch (cmd) {
    case CMD_WAIT:
      break;
    case CMD_BUTTON_CHANGE:
      int currentState = digitalRead(btnpin);
      if (currentState != buttonState) {
        if (buttonState == LOW && currentState == HIGH) {
          long duration = millis() - startPress;
          if (duration < 1000) {
            //Serial.println("short press - toggle relay");
            //toggle();
            ui.nextFrame();
          } else if (duration < 5000) {
            //Serial.println("medium press - reset");
            dis_stat_prog("medium press - reset");
            restart();
          } else if (duration < 60000) {
            //Serial.println("long press - reset settings");
            dis_stat_prog("long press - reset settings");

            reset();
          }
        } else if (buttonState == HIGH && currentState == LOW) {
          startPress = millis();
        }
        buttonState = currentState;
      }
      break;
  }


  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.

    check_ota();

    if ( go_metro.check() == 1 ) {
      do_sensor();
    }

  }
}

void do_sensor() {

  set_rgb(0, 1024, 0);

  get_bme280();

  set_rgb(0, 0, 0);

}



void set_rgb(int iv_red, int iv_green, int iv_blue) {

  int lv_green = iv_green * 0.8;
  int lv_red = iv_red;
  int lv_blue = iv_blue;

  if ( LDRValue < gv_LDRThres ) {
    lv_green = 0;
    lv_red = 0;
    lv_blue = 0;
  }

  analogWrite(ledpinrt, lv_red);
  analogWrite(ledpingn, lv_green);
  analogWrite(ledpinbl, lv_blue);
}
