/*
# File
AQM1602XA.h

# Version
1.2.0

# Target device

AQM1602XA-RN-GBW
Xiamen Zettler Electronics Co., Ltd. 
https://akizukidenshi.com/catalog/g/gP-08779/

AQM0802A-RN-GBW
Xiamen Zettler Electronics Co., Ltd. 
https://akizukidenshi.com/catalog/g/g106669

# Author
Suns & Moon Laboratory
https://www.s-m-l.org
*/
#ifndef AQM1602XA_h
#define AQM1602XA_h
#include <Arduino.h>
#include <Wire.h>

class AQM1602XA
{
  private:
    TwoWire *wire;
    bool flag_cursor_on;
    bool flag_blink_on;
    void init_lcd();
    void writeCommand(byte t_command);
    void cursorOn(bool cursor_on , bool blink_on = false);
  public:
    void begin(TwoWire *prm_wire);

    void on();
    void off();
    void contrast(int val);

    int width();
    int height();

    void clear();
    void home();
    void setCursor(int col, int row);
    void cursor();
    void noCursor();
    void blink();
    void noBlink();

    void writeCGRAM(int cgram_adr, byte* data, int count);

    void write(char ch);
    void writeData(byte t_data);
    void writeBufferData(byte *buf, int len);

    void print(char *str);
    void print(int val);
};

#endif
