/*
# File
AQM1602XA.h

# Version
1.0.0

# Target device
Xiamen Zettler Electronics Co., Ltd. AQM1602XA-RN-GBW
https://akizukidenshi.com/catalog/g/gP-08779/

# Author
Suns & Moon Laboratory
https://www.s-m-l.org
*/
#ifndef AQM1602XA_h
#define AQM1602XA_h
#include "Arduino.h"

class AQM1602XA
{
  private:
    bool flag_cursor_on;
    bool flag_blink_on;
    void init_lcd();
    void writeCommand(byte t_command);
    void writeData(byte t_data);
    void cursorOn(bool cursor_on , bool blink_on = false);
  public:
    void begin();
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

    void write(char ch);
    void print(char *str);
    void print(int val);
};

#endif
