#include <Arduino.h>
#include <Wire.h>
#include <iarduino_RTC.h>

#include "render.h"
#include "sensors.h"
#include "buttonMinim.h"

iarduino_RTC rtc(RTC_DS1307);

uint32_t bmpPollingTimer, mqPollingTimer, clockUpdateTimer, dotUpdateTimer;
int humidity, preassure, ppm;
float temperature;
bool showDots;

void updateBMPReadings(){
  temperature = getTemperature();
  preassure = getPreassure();
  humidity = getHumidity();  
}

void renderSensors(){
  drawTemperature(temperature);
  drawPreassure(preassure);
  drawHumidity(humidity);
  drawPPM(ppm);
}

buttonMinim bigDigBtn(D4);

void setup() {
  setupSensors();
  setupLCD();

  updateBMPReadings();
  ppm = getPPM();
  
  renderSensors();
  rtc.begin();
  rtc.gettime();
}

void loop() {
  if (bigDigBtn.holded()){
    bigDig = !bigDig;
    lcd.clear();
    reloadClock();
    drawClock(rtc.Hours, rtc.minutes, rtc.seconds);
    drawDate(rtc.year, rtc.month, rtc.day, rtc.weekday);
    renderSensors();
  }
  
  if (millis() - bmpPollingTimer > 5000){
    bmpPollingTimer = millis();
    updateBMPReadings();
    renderSensors();
  }

  if (millis() - mqPollingTimer > 60000){
    mqPollingTimer = millis();
    ppm = getPPM();
    renderSensors();
  }
  if (millis() - dotUpdateTimer > 500){
    dotUpdateTimer = millis();
    drawDots(showDots);
    showDots = !showDots;
  }
  
  if (millis() - clockUpdateTimer > 1000){
    rtc.gettime();
    clockUpdateTimer = millis();
    drawClock(rtc.Hours, rtc.minutes, rtc.seconds);
    drawDate(rtc.year, rtc.month, rtc.day, rtc.weekday);
  }
}
