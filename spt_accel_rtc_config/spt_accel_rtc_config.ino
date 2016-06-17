#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 RTC;

void setup() {
  Serial.begin(115200);
  
  RTC.begin();
  if (!RTC.isrunning()) {
    Serial.println("RTC is not running");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  Serial.println("Time is now set, and clock is running");
}

void loop() {
  printTime();
}

void printTime() {
  DateTime now = RTC.now();
  Serial.print("Unix Timestamp: ");
  Serial.println(now.unixtime());
  delay(1000);
}

