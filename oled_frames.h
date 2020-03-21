void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  String timenow = String(hour()) + ":" + twoDigits(minute()) + ":" + twoDigits(second());
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(clockCenterX + x , clockCenterY + y, timenow );
}

void DHT22Frame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  String DHT22val = String(gv_temp_bme280) + "°";
  display->drawString(clockCenterX + x , 16 + y, DHT22val );
  DHT22val = String(gv_humi_bme280) + "%";
  display->drawString(clockCenterX + x , clockCenterY + y, DHT22val );

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);
}

void analogClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  //  ui.disableIndicator();

  // Draw the clock face
  //  display->drawCircle(clockCenterX + x, clockCenterY + y, clockRadius);
  display->drawCircle(clockCenterX + x, clockCenterY + y, 2);
  //
  //hour ticks
  for ( int z = 0; z < 360; z = z + 30 ) {
    //Begin at 0° and stop at 360°
    float angle = z ;
    angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
    int x2 = ( clockCenterX + ( sin(angle) * clockRadius ) );
    int y2 = ( clockCenterY - ( cos(angle) * clockRadius ) );
    int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 8 ) ) ) );
    display->drawLine( x2 + x , y2 + y , x3 + x , y3 + y);
  }

  // display second hand
  float angle = second() * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  int x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  int y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 5 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  //
  // display minute hand
  angle = minute() * 6 ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 4 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);
  //
  // display hour hand
  angle = hour() * 30 + int( ( minute() / 12 ) * 6 )   ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x3 = ( clockCenterX + ( sin(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  y3 = ( clockCenterY - ( cos(angle) * ( clockRadius - ( clockRadius / 2 ) ) ) );
  display->drawLine( clockCenterX + x , clockCenterY + y , x3 + x , y3 + y);

  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);
}


void IPFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  String ipaddr = WiFi.localIP().toString();
  display->drawString(clockCenterX + x , 16 + y, ipaddr );
  display->drawString(clockCenterX + x , 24 + y, WiFi.SSID() );
  String lv_LDRThres_s = String(gv_LDRThres);
  String lv_LDRVal_s = String(LDRValue);
  display->drawString(clockCenterX + x , 32 + y, lv_LDRThres_s + " / " + lv_LDRVal_s );

}

// This array keeps function pointers to all frames
// frames are the single views that slide in
// FrameCallback frames[] = { analogClockFrame, digitalClockFrame, DHT22Frame };
//FrameCallback frames[] = { analogClockFrame, IPFrame, DHT22Frame };
FrameCallback frames[] = { DHT22Frame, IPFrame };

// how many frames are there?
int frameCount = 2;


void clockOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  String timenow = String(hour()) + ":" + twoDigits(minute()) + ":" + twoDigits(second());
  display->drawString(128, 0, timenow);
  //String datenow =
}

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = { clockOverlay };
int overlaysCount = 1;

