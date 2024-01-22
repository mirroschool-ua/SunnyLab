#include <iarduino_RTC.h>

iarduino_RTC watch(RTC_DS1307);

void setup() {
  delay(300);
  watch.begin();
  watch.settime(__TIMESTAMP__); 
}

void loop() {

}
