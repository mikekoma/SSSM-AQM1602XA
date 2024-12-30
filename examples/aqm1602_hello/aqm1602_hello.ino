#include "AQM1602XA.h"

#define I2C0_PIN_SDA 16  // I2C0 PiPico,TEMP4CH
#define I2C0_PIN_SCL 17  // I2C0 PiPico,TEMP4CH
AQM1602XA lcd;

void setup() {

  Wire.setSDA(I2C0_PIN_SDA);
  Wire.setSCL(I2C0_PIN_SCL);
  Wire.begin();
  lcd.begin(&Wire);

  int contrast = 32;
  lcd.contrast(contrast);

  lcd.clear();
  lcd.home();
  lcd.print((char *)"Hello! AQM1602XA");
  // lcd.cursor();
  // lcd.blink();
}

int count = 0;

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(count++);
  delay(1000);
}