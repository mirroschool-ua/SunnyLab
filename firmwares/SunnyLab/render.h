#ifndef _RENDER_H
#define _RENDER_H

#include <LiquidCrystal_I2C.h>

bool bigDig;

LiquidCrystal_I2C lcd(0x27, 20, 4);

byte row5[8] = {0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
byte row3[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
byte row2[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};

uint8_t UB[8] = {0b11111,  0b11111,  0b11111,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000};
uint8_t UMB[8] = {0b11111,  0b11111,  0b11111,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
uint8_t LMB[8] = {0b11111,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
uint8_t LM2[8] = {0b11111,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000};
uint8_t UT[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b00000,  0b00000,  0b00000};

uint8_t BIG_DIGITS[11][12] = {
  {255, 0, 255, 255, 32, 255, 255, 32, 255, 255, 3, 255},
  {32, 255, 32, 32, 255, 32, 32, 255, 32, 32, 255, 32},
  {0, 0, 255, 1, 1, 255, 255, 2, 2, 255, 3, 3},
  {0, 0, 255, 1, 1, 255, 2, 2, 255, 3, 3, 255},
  {255, 32, 255, 255, 1, 255, 2, 2, 255, 32, 32, 255},
  {255, 0, 0, 255, 1, 1, 2, 2, 255, 3, 3, 255},
  {255, 0, 0, 255, 1, 1, 255, 2, 255, 255, 3, 255},
  {0, 0, 255, 32, 32, 255, 32, 255, 32, 32, 255, 32},
  {255, 0, 255, 255, 1, 255, 255, 2, 255, 255, 3, 255},
  {255, 0, 255, 255, 1, 255, 2, 2, 255, 3, 3, 255},
  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}
};

uint8_t SMALL_DIGITS[11][6] = {
  {255, 1, 255, 255, 2, 255},
  {32, 255, 32, 32, 255, 32},
  {3, 3, 255, 255, 4, 4},
  {3, 3, 255, 4, 4, 255},
  {255, 0, 255, 5, 5, 255},
  {255, 3, 3, 4, 4, 255},
  {255, 3, 3, 255, 4, 255},
  {1, 1, 255, 32, 255, 32},
  {255, 3, 255, 255, 4, 255},
  {255, 3, 255, 4, 4, 255},
  {32, 32, 32, 32, 32, 32}
};

static const char *dayNames[]  = {
  "Su",
  "Mo",
  "Tu",
  "We",
  "Th",
  "Fr",
  "Sa",
};

void drawDig(byte dig, byte x, byte y) {
  uint8_t *digit, rows;
  
  if (bigDig) {
    digit = BIG_DIGITS[dig];
    rows = 4;
  } else {
    digit = SMALL_DIGITS[dig];
    rows = 2;
  }

  for (int row=0; row<rows; row++){
    lcd.setCursor(x, y+row);
    for (int col=0; col<3; col++){
      lcd.write(digit[row*3 + col]);
    }
  }
}

void drawClock(byte hours, byte minutes, byte seconds) {
  if (hours > 23 || minutes > 59) return;
  if (hours / 10 == 0) drawDig(0, 0, 0);
  else drawDig(hours / 10, 0, 0);
  drawDig(hours % 10, 4, 0);
  
  drawDig(minutes / 10, 8, 0);
  drawDig(minutes % 10, 12, 0);

  if (!bigDig){
    lcd.setCursor(15, 1);
    if (seconds < 10) {
      lcd.print(" ");
    }
    lcd.print(seconds);
  }
}

void drawDots(bool showDots){
    byte dotChar;    
    if (showDots){
      dotChar = 165;
    } else {
      dotChar = 32;
    }
    
    lcd.setCursor(7, 0);
    lcd.write(dotChar);

    if (bigDig){
      lcd.setCursor(7, 3);
    } else {
      lcd.setCursor(7, 1);
    }
    lcd.write(dotChar);
}

void drawDate(uint16_t year, byte month, byte day, byte weekday){
  if (!bigDig) {
    lcd.setCursor(15, 0);
    if (day < 10) lcd.print(0);
    lcd.print(day);
    lcd.print(".");
    if (month < 10) lcd.print(0);
    lcd.print(month);

    if (true) {
      lcd.setCursor(16, 3);
      lcd.print(year);
    } else {
      lcd.setCursor(18, 1);
      int dayofweek = now.DayOfWeek();
      lcd.print(dayNames[dayofweek]);
    }
  }
}

void drawTemperature(float temperature){
  if (bigDig){
    lcd.setCursor(15, 0);
  } else {
    lcd.setCursor(0, 2);
  }
  
  lcd.print(String(temperature, 1));
  lcd.write(223);
}

void drawHumidity(int humidity){
  if (bigDig){
    lcd.setCursor(15, 1);
  } else {
    lcd.setCursor(0, 3);
  }

  lcd.print(String(humidity) + " %");
}

void drawPPM(int ppm){
  char ppmBuf[9];
  
  if (bigDig){
    lcd.setCursor(15, 2);
    sprintf(ppmBuf, "%-4.1dp", ppm);
  } else {
    lcd.setCursor(8, 2);
    sprintf(ppmBuf, "%-4.1dppm", ppm);
  }
  
  lcd.print(ppmBuf);
}

void drawPreassure(int pressure){
  char pressureBuf[9];
  
  if (bigDig){
    lcd.setCursor(15, 3);
    sprintf(pressureBuf, "%-3.1dmm", pressure);
  } else {
    lcd.setCursor(8, 3);
    sprintf(pressureBuf, "%-4.1dmm", pressure);
  }
  
  lcd.print(pressureBuf);
}

void reloadClock() {
  if (bigDig) {
    lcd.createChar(0, UT);
    lcd.createChar(1, row3);
    lcd.createChar(2, UB);
    lcd.createChar(3, row5);
  }
  else {
    lcd.createChar(0, row2);
    lcd.createChar(1, UB);
    lcd.createChar(2, row3);
    lcd.createChar(3, UMB);
    lcd.createChar(4, LMB);
    lcd.createChar(5, LM2);
  }
}

void setupLCD(){
  lcd.init();
  lcd.backlight();
  reloadClock();
}

#endif
