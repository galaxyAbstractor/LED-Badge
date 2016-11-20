#include <ArduinoJson.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "font.h"

HTTPClient http;
SSD1306  display(0x3c, 0, 2);
String twitter = "http://test-galaxyabstractor.c9users.io";
StaticJsonBuffer<3000> jsonBuffer;

String rows[2];
int offsets[2];
int left = 0;

void setup() {
  Serial.begin(115200);

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(Liberation_Mono_24);
  WiFi.begin();
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
    }
}

void scroller() {
  for(int i = 0; i < 2; i++) {
    if(rows[i].length() > 0) {
      int offset = offsets[i];
      display.drawString(offset, i * 30, rows[i].substring(0, 15));
  
      if(offset < -13) {
       rows[i] = rows[i].substring(1);
        if(rows[i].length() < 1) {
          left--;
        }
        offsets[i] = 0;
      } else {
        offsets[i] = offset - 1;
      }
    }
    
  }
  
}

void tick () {
  display.clear();
  
  if(left == 0) {
   
    http.begin(twitter, 80, "/");
    int httpCode = http.GET();
    String response = http.getString();

    JsonObject& root = jsonBuffer.parseObject(response);

    for(int i = 0; i < 2; i++) {
      const char* data = root["data"][i];
      rows[i] = String(data);
      offsets[i] = (i * 14);
      
    }
    left = 2;
  }
  scroller();
  
  display.display();
}

unsigned long prevFrameTime = 0L;

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long t = millis();
  
  if((t - prevFrameTime) >= (20L)) {
    tick();
    prevFrameTime = millis();
  }
}
