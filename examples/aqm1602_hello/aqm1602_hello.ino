#include "AQM1602XA.h"

AQM1602XA lcd;

void setup() {

  lcd.begin();

  int contrast = 32;
  lcd.contrast(contrast);

  lcd.clear();
  lcd.home();
  lcd.print("Hello! AQM1602XA");
  // lcd.cursor();
  // lcd.blink();
}

int count = 0;

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(count++);
  delay(1000);
}