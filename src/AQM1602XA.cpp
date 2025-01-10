/*
# File
AQM1602XA.cpp

# Version
1.1.0

# Target device
Xiamen Zettler Electronics Co., Ltd. AQM1602XA-RN-GBW
https://akizukidenshi.com/catalog/g/gP-08779/

# Author
Suns & Moon Laboratory
https://www.s-m-l.org
*/
#include <Arduino.h>
#include <wire.h>
#include "AQM1602XA.h"

#define DEVICE_ADDRESS 0x3E

#define INST_CLEAR_DISPLAY 0x01
#define INST_RETURN_HOME 0x02
#define INST_DISPLAY_OFF 0x08  // bit2:display=off bit1:cursor=off bit0:cursor position=off
#define INST_DISPLAY_ON 0x0C   // bit2:display=on  bit1:cursor=off bit0:cursor position=off
#define INST_FUNCTION_SET_IS0 0x38
#define INST_FUNCTION_SET_IS1 (INST_FUNCTION_SET_IS0 | 0x01)
#define INST_SET_DDRAM_ADR 0x80
#define INST_SET_CGRAM_ADR 0x40

void AQM1602XA::writeCommand(byte t_command) {
  wire->beginTransmission(DEVICE_ADDRESS);
  wire->write(0x00);  // Co=0,RS=0(Instruction)
  wire->write(t_command);
  wire->endTransmission();
  delay(10);
}

void AQM1602XA::writeData(byte t_data) {
  wire->beginTransmission(DEVICE_ADDRESS);
  wire->write(0x40);  // Co=0,RS=1(Data)
  wire->write(t_data);
  wire->endTransmission();
  delayMicroseconds(30);
}

void AQM1602XA::writeBufferData(byte *buf, int len) {
  wire->beginTransmission(DEVICE_ADDRESS);
  wire->write(0x40);  // Co=0,RS=1(Data)
  for (int i = 0; i < len; i++) {
    wire->write(*buf);
    buf++;
  }
  wire->endTransmission();
  delayMicroseconds(30);
}

void AQM1602XA::cursorOn(bool cursor_on, bool blink_on) {
  uint8_t val;
  val = 0x0c;  //display on
  val |= cursor_on ? 0x02 : 0;
  val |= blink_on ? 0x01 : 0;
  writeCommand(val);
  delayMicroseconds(30);
}

void AQM1602XA::init_lcd() {
  delay(100);
  writeCommand(INST_FUNCTION_SET_IS0);  // FUNCTION SET
  delayMicroseconds(30);
  writeCommand(INST_FUNCTION_SET_IS1);  // IS=1 ここから
  delayMicroseconds(30);
  writeCommand(0x14);  // INT OSC FREQUENCY
  delayMicroseconds(30);
  writeCommand(0x7C);  // CONTRAST SET 0,1,2,3
  delayMicroseconds(30);
  writeCommand(0x54);  // CONTRAST SET 4,5
  delayMicroseconds(30);
  writeCommand(0x6C);  // F0LLOWER CONTROL
  delayMicroseconds(30);
  writeCommand(INST_FUNCTION_SET_IS0);  // IS=1 ここまで
  delayMicroseconds(30);
  writeCommand(0x0C);  // Display ON
  delayMicroseconds(30);
  writeCommand(0x01);  // clear Display
  delayMicroseconds(1020);
  writeCommand(0x06);  // Entry Mode
  delayMicroseconds(30);
}

void AQM1602XA::begin(TwoWire *prm_wire) {
  wire = prm_wire;

  flag_cursor_on = false;
  flag_blink_on = false;

  init_lcd();

  wire->beginTransmission(DEVICE_ADDRESS);
  wire->write(0x20);
  wire->write(0x01);
  wire->endTransmission();
}

void AQM1602XA::on() {
  writeCommand(INST_DISPLAY_ON);  // Display ON
  delayMicroseconds(30);
}

void AQM1602XA::off() {
  writeCommand(INST_DISPLAY_OFF);  // Display OFF
  delayMicroseconds(30);
}

void AQM1602XA::contrast(int val) {
  writeCommand(INST_FUNCTION_SET_IS0);  //注意：ここIS0⇒IS1しないと表示しない。
  delayMicroseconds(30);
  writeCommand(INST_FUNCTION_SET_IS1);  //注意：ここIS0⇒IS1しないと表示しない。
  delayMicroseconds(30);
  writeCommand(0x70 | (byte)(val & 0x0F));  // CONTRAST SET 0,1,2,3
  delayMicroseconds(30);
  writeCommand(0x54 | (byte)(val >> 4 & 0x03));  // CONTRAST SET 4,5
  delayMicroseconds(30);
  // writeCommand(0x06);  // Entry Mode
  // delayMicroseconds(30);
}

int AQM1602XA::width() {
  return 16;
}

int AQM1602XA::height() {
  return 2;
}

void AQM1602XA::clear() {
  writeCommand(INST_CLEAR_DISPLAY);
}

void AQM1602XA::home() {
  writeCommand(INST_RETURN_HOME);
}

void AQM1602XA::setCursor(int col, int row) {
  writeCommand(INST_SET_DDRAM_ADR + (row ? 0x40 : 0) + col);
}

void AQM1602XA::cursor() {
  flag_cursor_on = true;
  cursorOn(flag_cursor_on, flag_blink_on);
}

void AQM1602XA::noCursor() {
  flag_cursor_on = false;
  cursorOn(flag_cursor_on, flag_blink_on);
}

void AQM1602XA::blink() {
  flag_blink_on = true;
  cursorOn(flag_cursor_on, flag_blink_on);
}

void AQM1602XA::noBlink() {
  flag_blink_on = false;
  cursorOn(flag_cursor_on, flag_blink_on);
}

void AQM1602XA::write(char ch) {
  writeData(ch);
}

void AQM1602XA::print(char *str) {
  int count = strlen(str);
  writeBufferData((byte*)str, count);
}

void AQM1602XA::print(int val) {
  char buf[8];
#if 1
  String str = String(val);
  str.toCharArray(buf, sizeof(buf));
#else
  sprintf(buf, "%d", val);
#endif
  print(buf);
}

void AQM1602XA::writeCGRAM(int cgram_adr, byte *data, int count) {
  writeCommand(INST_FUNCTION_SET_IS0);
  delayMicroseconds(30);
  writeCommand(INST_SET_CGRAM_ADR + (0x3F & cgram_adr));
  delayMicroseconds(30);
  writeBufferData(data, count);
}