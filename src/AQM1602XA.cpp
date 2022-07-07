/*
# File
AQM1602XA.cpp

# Version
1.0.0

# Target device
Xiamen Zettler Electronics Co., Ltd. AQM1602XA-RN-GBW
https://akizukidenshi.com/catalog/g/gP-08779/

# Author
Suns & Moon Laboratory
https://www.s-m-l.org
*/
#include <Arduino.h>
#include <Wire.h>
#include "AQM1602XA.h"

#define DEVICE_ADDRESS  0x3E

void AQM1602XA::writeCommand(byte t_command)
{
  Wire.begin();
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(0x00);
  Wire.write(t_command);
  Wire.endTransmission();
  delay(10);
}

void AQM1602XA::writeData(byte t_data)
{
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(0x40);
  Wire.write(t_data);
  Wire.endTransmission();
  delayMicroseconds(30);
}

void AQM1602XA::cursorOn(bool cursor_on, bool blink_on)
{
  uint8_t val;
  val = 0x0c;//display on
  val |= cursor_on ? 0x02 : 0;
  val |= blink_on ? 0x01 : 0;
  writeCommand(val);
  delayMicroseconds(30);
}

void AQM1602XA::init_lcd()
{
  delay(100);
  writeCommand(0x38); // FUNCTION SET
  delayMicroseconds(30);
  writeCommand(0x39); // IS=1
  delayMicroseconds(30);
  writeCommand(0x14); // INT OSC FREQUENCY
  delayMicroseconds(30);
  writeCommand(0x7C); // CONTRAST SET 0,1,2,3
  delayMicroseconds(30);
  writeCommand(0x54); // CONTRAST SET 4,5
  delayMicroseconds(30);
  writeCommand(0x6C); // F0LLOWER CONTROL
  delayMicroseconds(30);
  writeCommand(0x38); // IS=0
  delayMicroseconds(30);
  writeCommand(0x0C); // Display ON
  delayMicroseconds(30);
  writeCommand(0x01); // clear Display
  delayMicroseconds(1020);
  writeCommand(0x06); // Entry Mode
  delayMicroseconds(30);
}

void AQM1602XA::begin()
{
  flag_cursor_on = false;
  flag_blink_on = false;

  init_lcd();

  //  writeCommand(0x02);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(0x20);
  Wire.write(0x01);
  Wire.endTransmission();
}

void AQM1602XA::contrast(int val)
{
  writeCommand(0x38); // FUNCTION SET
  delayMicroseconds(30);
  writeCommand(0x39); // IS=1
  delayMicroseconds(30);
  writeCommand(0x70 | (byte)(val & 0x0F)); // CONTRAST SET 0,1,2,3
  delayMicroseconds(30);
  writeCommand(0x54 | (byte)(val >> 4 & 0x03)); // CONTRAST SET 4,5
  delayMicroseconds(30);
  writeCommand(0x06); // Entry Mode
  delayMicroseconds(30);
}

int AQM1602XA::width()
{
  return 16;
}

int AQM1602XA::height()
{
  return 2;
}

void AQM1602XA::clear()
{
  writeCommand(0x01);
}

void AQM1602XA::home()
{
  writeCommand(0x02);
}

void AQM1602XA::setCursor(int col, int row)
{
  writeCommand(0x80 + (row ? 0x40 : 0) + col);
}

void AQM1602XA::cursor()
{
  flag_cursor_on = true;
  cursorOn(flag_cursor_on, flag_blink_on);
}

void AQM1602XA::noCursor()
{
  flag_cursor_on = false;
  cursorOn(flag_cursor_on, flag_blink_on);
}

void AQM1602XA::blink()
{
  flag_blink_on = true;
  cursorOn(flag_cursor_on, flag_blink_on);
}

void AQM1602XA::noBlink()
{
  flag_blink_on = false;
  cursorOn(flag_cursor_on, flag_blink_on);
}

void AQM1602XA::write(char ch)
{
  writeData(ch);
}

void AQM1602XA::print(char *str)
{
  while (*str) {
    writeData(*str++);
  }
}

void AQM1602XA::print(int val)
{
  char buf[8];
#if 1
  String str = String(val);
  str.toCharArray(buf, sizeof(buf));
#else
  sprintf(buf, "%d", val);
#endif
  print(buf);
}
