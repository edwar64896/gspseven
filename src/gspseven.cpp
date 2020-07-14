#include "gspseven.h"

// ... write a value into a max7219 register 
// See MAX7219 Datasheet, Table 1, page 6
void gspSeven::set_register(byte reg, byte value)  
{
    digitalWrite(_cs, LOW);
    digitalWrite(_clk, LOW);

    shiftOut(_data, _clk, MSBFIRST, reg);
    shiftOut(_data, _clk, MSBFIRST, value);

    digitalWrite(_cs, HIGH);
}

void gspSeven::sendRegistersToAll(byte reg, byte value)  
{
    
    digitalWrite(_cs, LOW);

    for (int i=0;i<8;i++) {

        shiftOut(_data, _clk, MSBFIRST, reg);
        shiftOut(_data, _clk, MSBFIRST, value);
    }
    digitalWrite(_clk,LOW);
    digitalWrite(_cs, HIGH);
    
}

void gspSeven::set_register(uint8_t display, byte reg, byte value)  
{
    digitalWrite(_cs, LOW);

    for (int i=0;i<1;i++) {
        shiftOut(_data, _clk, MSBFIRST, 0x00);
        shiftOut(_data, _clk, MSBFIRST, 0x00);
    }

    shiftOut(_data, _clk, MSBFIRST, reg);
    shiftOut(_data, _clk, MSBFIRST, value);

    for (int i=0;i<display;i++) {
    //    digitalWrite(_clk,LOW);
        shiftOut(_data, _clk, MSBFIRST, 0x00);
        shiftOut(_data, _clk, MSBFIRST, 0x00);
    }
    
    digitalWrite(_cs, HIGH);
}

void gspSeven::resetAllDisplays()  
{

    //sendRegistersToAll(MAX7219_REG_SHUTDOWN, OFF);   // turn off display
    //sendRegistersToAll(MAX7219_REG_DISPTEST, OFF);   // turn off test mode
    //sendRegistersToAll(MAX7219_REG_INTENSITY, 0x02); // display intensity

    for (int i=0;i<8;i++) {
        resetDisplay(i);
    }
}

void gspSeven::resetDisplay(int display)  
{
    set_register(display,MAX7219_REG_SHUTDOWN, OFF);   // turn off display
    set_register(display,MAX7219_REG_DISPTEST, OFF);   // turn off test mode
    set_register(display,MAX7219_REG_INTENSITY, 0x0D); // display intensity

    delay(100);
}

void gspSeven::displayNums(int display, String inStr)  
{
    set_register(display,   MAX7219_REG_SHUTDOWN,     OFF);  // turn off display
    set_register(display,   MAX7219_REG_SCANLIMIT,    6);   // scan limit 8 digits
    set_register(display,   MAX7219_REG_DECODE,       0b11111111); // decode all digits

    //set_register(1, inStr.charAt(7));
    set_register(display,   1,    0x0F);
    set_register(display,   2,    inStr.charAt(6));
    set_register(display,   3,    inStr.charAt(5));
    set_register(display,   4,    inStr.charAt(4));
    set_register(display,   5,    inStr.charAt(2) | DP); // plus decimal point
    set_register(display,   6,    inStr.charAt(1));
    set_register(display,   7,    inStr.charAt(0));
    set_register(display,   8,    0x0F);
    
    set_register(display,   MAX7219_REG_SHUTDOWN, ON);
    //    sendRegistersToAll(MAX7219_REG_SHUTDOWN, ON);
}


void gspSeven::testBounce(int display)  
{
    set_register(display,MAX7219_REG_SHUTDOWN, OFF);  // turn off display
    set_register(display,MAX7219_REG_SCANLIMIT, 7);   // scan limit 8 digits
    set_register(display,MAX7219_REG_DECODE, 0b11111111); // decode all digits    
    set_register(display,MAX7219_REG_SHUTDOWN, ON);   // Turn on display
}

void gspSeven::setAllIntensity(uint8_t intensity){
    sendRegistersToAll(MAX7219_REG_INTENSITY, intensity & 0x0F);  // turn off display   
}

void gspSeven::clearAllDisplays()  
{
    for (int i=0;i<8;i++) {
        clearDisplay(i);
    }

}

void gspSeven::clearDisplay(int display)  
{
    set_register(display,MAX7219_REG_SHUTDOWN, OFF);  // turn off display
    set_register(display,MAX7219_REG_SCANLIMIT, 7);   // scan limit 8 digits
    set_register(display,MAX7219_REG_DECODE, 0b11111111); // decode all digits

    for (int i=1;i<9;i++) {
        set_register(display,i, 0x0F);
    }

    set_register(display,MAX7219_REG_SHUTDOWN, ON);   // Turn on display
}

void gspSeven::displayNums(String inStr)  
{
    set_register(MAX7219_REG_SHUTDOWN, OFF);  // turn off display
    set_register(MAX7219_REG_SCANLIMIT, 7);   // scan limit 8 digits
    set_register(MAX7219_REG_DECODE, 0b11111111); // decode all digits

    //set_register(1, inStr.charAt(7));
    set_register(1, 0x0F);
    set_register(2, inStr.charAt(6));
    set_register(3, inStr.charAt(5));
    set_register(4, inStr.charAt(4));
    set_register(5, inStr.charAt(2) | DP); // plus decimal point
    set_register(6, inStr.charAt(1));
    set_register(7, inStr.charAt(0));
    set_register(8, 0x0F);
    
    set_register(MAX7219_REG_SHUTDOWN, ON);   // Turn on display
}

gspSeven::gspSeven(int data, int clock, int chipSelect, int nDisplays=1):
_data(data),_clk(clock),_cs(chipSelect),_nDisplays(nDisplays)
{
    pinMode(_data,OUTPUT);  //12
    pinMode(_clk,OUTPUT);   //10
    pinMode(_cs,OUTPUT);    //11
    digitalWrite(_cs, HIGH);
    digitalWrite(_clk, LOW);
    

    sendRegistersToAll(OP_SHUTDOWN,OFF);
    sendRegistersToAll(OP_DISPLAYTEST,OFF);
    sendRegistersToAll(OP_INTENSITY,0X0A);
    clearAllDisplays();
}

