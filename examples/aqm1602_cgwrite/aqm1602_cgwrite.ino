#include "AQM1602XA.h"

#define CUSTOM_PIN
#ifdef CUSTOM_PIN
#define I2C0_PIN_SDA 16  // I2C0 PiPico,TEMP4CH
#define I2C0_PIN_SCL 17  // I2C0 PiPico,TEMP4CH
#endif

/*
XIAO RP2040
D4 -> SDA
D5 -> SCL
*/

AQM1602XA lcd;

void setup() {

#ifdef CUSTOM_PIN
  Wire.setSDA(I2C0_PIN_SDA);
  Wire.setSCL(I2C0_PIN_SCL);
#endif
  Wire.begin();
  lcd.begin(&Wire);

  int contrast = 32;
  lcd.contrast(contrast);

  lcd.clear();
  lcd.home();
  lcd.cursor();
  lcd.blink();

  uint8_t cgdata1[] = {
    //(1)
         0x0E  //01110
    ,    0x1B  //11011
    ,    0x13  //10011
    ,    0x1B  //11011
    ,    0x1B  //11011
    ,    0x1B  //11011
    ,    0x11  //10001
    ,    0x0E  //01110
  };
  uint8_t cgdata2[] = {
    //(2)
         0x0E  //01110
    ,    0x1B  //11011
    ,    0x15  //10101
    ,    0x1D  //11101
    ,    0x1B  //11011
    ,    0x17  //10111
    ,    0x11  //10001
    ,    0x0E  //01110
  };
  uint8_t cgdata3[] = {
    //(3)
         0x0E  //01110
    ,    0x11  //10001
    ,    0x1D  //11101
    ,    0x1B  //11011
    ,    0x1D  //11101
    ,    0x1D  //11101
    ,    0x13  //10011
    ,    0x0E  //01110
  };
  uint8_t cgdata4[] = {
    //(4)
         0x0E  //01110
    ,    0x15  //11101
    ,    0x15  //10101
    ,    0x15  //10101
    ,    0x11  //10001
    ,    0x1D  //11101
    ,    0x1D  //11101
    ,    0x0E  //01110
  } ;

  lcd.writeCGRAM(+0, cgdata1, sizeof(cgdata1));
  lcd.writeCGRAM(+8, cgdata2, sizeof(cgdata2));
  lcd.writeCGRAM(16, cgdata3, sizeof(cgdata3));
  lcd.writeCGRAM(24, cgdata4, sizeof(cgdata4));
  
  lcd.setCursor(0, 0);
  for (int i = 0; i < 8; i++) {
    lcd.writeData(i);
  }

  lcd.setCursor(0, 1);
  for (int i = 0; i < 8; i++) {
    lcd.writeData(i + 8);
  }
}

int count = 0;

void loop() {
  lcd.setCursor(0, 1);
  lcd.print(count++);
  delay(1000);
}