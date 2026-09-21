#ifndef _SENSORS_H
#define _SENSORS_H

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <HTU21D.h>
#include <MQ135.h>

int previousPPM = -1;

Adafruit_BMP280 bmp;
HTU21D htu;
MQ135 gasSensor = MQ135(A0);

void setupSensors(){
  bmp.begin(0x76);
  htu.begin();
}

int getPreassure(){
  return bmp.readPressure() * 0.00750062;
}

int getHumidity(){
  return htu.readHumidity();
}

float getTemperature(){
  return htu.readTemperature();
}

int getPPM(){
  int16_t ppm = gasSensor.getPPM();
 
  if (ppm > 0){
    previousPPM = ppm;
  }
  
  return previousPPM;
}

#endif
