#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "font.h"

HTTPClient http;
SSD1306  display(0x3c, 0, 2);
String twitter = "";

String rows[2];
int offsets[2];
int left = 0;

void setup() {
  Serial.begin(115200);

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(Liberation_Mono_24);
 
}

void scroller() {
   display.clear();
  
  for(int i = 0; i < 2; i++) {
    if(rows[i].length() > 0) {
      int offset = offsets[i];
      display.drawString(offset, 5 + (i * 30), rows[i].substring(0, 15));
  
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

  display.display();
}

void getTweets() {
  if(WiFi.status() != WL_CONNECTED) {
      WiFi.begin();
    }
 
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(200);
    }
    
    http.begin(twitter);
    http.setUserAgent("Galaxy Badge Project");
    int httpCode = http.GET();
    String response = http.getString();
    
    int firstIndex = response.indexOf('||||');
    
    rows[0] = response.substring(0, firstIndex);
    rows[1] = response.substring(firstIndex+4);
    offsets[0] = display.getStringWidth(rows[0]);
    offsets[1] = display.getStringWidth(rows[0]) + 60;
    
    if(offsets[0] > 1 && offsets[1] > 60) {
      left = 2;
    } else if(offsets[0] > 1) {
      left = 1;
    }

}

void tick () {

  if(left == 0) {
    getTweets();
  }
  
  scroller();
}

unsigned long prevFrameTime = 0L;
bool a = true;
void loop() {
  if(a) {
    display.drawString(0, 1 * 30, "Booting");
    
    display.display();
  }
  a = false;
  // put your main code here, to run repeatedly:
  unsigned long t = millis();
  
  if((t - prevFrameTime) >= (10L)) {
    tick();
    prevFrameTime = millis();
  }
}
