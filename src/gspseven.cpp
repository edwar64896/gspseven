#include "gspseven.h"

// ... write a value into a max7219 register 
// See MAX7219 Datasheet, Table 1, page 6
void gspSeven::set_register(byte reg, byte value)  
{
    digitalWrite(_cs, LOW);
    //digitalWrite(_clk, LOW);

    shiftOut(_data, _clk, MSBFIRST, reg);
    shiftOut(_data, _clk, MSBFIRST, value);

    digitalWrite(_cs, HIGH);
}



void gspSeven::resetDisplay()  
{
    set_register(   MAX7219_REG_SHUTDOWN,   OFF);   // turn off display
    set_register(   MAX7219_REG_DISPTEST,   OFF);   // turn off test mode
    set_register(   MAX7219_REG_INTENSITY,  0x0D); // display intensity

    //delay(200);
        
    set_register(   MAX7219_REG_SHUTDOWN,   ON);   // turn off display
}

void gspSeven::displayNums(String inStr)  
{
    set_register(   MAX7219_REG_SHUTDOWN,     OFF);  // turn off display
    set_register(   MAX7219_REG_SCANLIMIT,    7);   // scan limit 8 digits
    set_register(   MAX7219_REG_DECODE,       0b11111111); // decode all digits


    set_register(   1,    0x0F);
    set_register(   2,    inStr.charAt(6));
    set_register(   3,    inStr.charAt(5));
    set_register(   4,    inStr.charAt(4));
    set_register(   5,    inStr.charAt(2) | DP); // plus decimal point
    set_register(   6,    inStr.charAt(1));
    set_register(   7,    inStr.charAt(0));
    set_register(   8,    0x0F);
    
    set_register(   MAX7219_REG_SHUTDOWN, ON);

}

void gspSeven::displayRtcDate(String inStr)  
{
    set_register(   MAX7219_REG_SHUTDOWN,     OFF);  // turn off display
    set_register(   MAX7219_REG_SCANLIMIT,    7);   // scan limit 8 digits
    set_register(   MAX7219_REG_DECODE,       0b11111111); // decode all digits


    set_register(   1,    inStr.charAt(7));
    set_register(   2,    inStr.charAt(6));
    set_register(   3,    inStr.charAt(5));
    set_register(   4,    inStr.charAt(4));
    set_register(   5,    inStr.charAt(3) | DP); // plus decimal point
    set_register(   6,    inStr.charAt(2));
    set_register(   7,    inStr.charAt(1) | DP);
    set_register(   8,    inStr.charAt(0));
    
    set_register(   MAX7219_REG_SHUTDOWN, ON);

}
void gspSeven::displayRtcTime(String inStr)  
{
    set_register(   MAX7219_REG_SHUTDOWN,     OFF);  // turn off display
    set_register(   MAX7219_REG_SCANLIMIT,    7);   // scan limit 8 digits
    set_register(   MAX7219_REG_DECODE,       0b11111111); // decode all digits


    set_register(   1,    inStr.charAt(7));
    set_register(   2,    inStr.charAt(6));
    set_register(   3,    0X0A);
    set_register(   4,    inStr.charAt(4));
    set_register(   5,    inStr.charAt(3)); // plus decimal point
    set_register(   6,    0x0A);
    set_register(   7,    inStr.charAt(1));
    set_register(   8,    inStr.charAt(0));
    
    set_register(   MAX7219_REG_SHUTDOWN, ON);

}

void gspSeven::displayAllNums(String inStr)  
{
    set_register(   MAX7219_REG_SHUTDOWN,     OFF);  // turn off display
    set_register(   MAX7219_REG_SCANLIMIT,    7);   // scan limit 8 digits
    set_register(   MAX7219_REG_DECODE,       0b11111111); // decode all digits


    for (int l=1;l<9;l++) {
        set_register(   l,    inStr.charAt(9-l));        
    }
    
    set_register(   MAX7219_REG_SHUTDOWN, ON);

}

void gspSeven::displayDigit(uint8_t unit, String inStr)  
{
    set_register(   MAX7219_REG_SHUTDOWN,     OFF);  // turn off display
    set_register(   MAX7219_REG_SCANLIMIT,    7);   // scan limit 8 digits
    set_register(   MAX7219_REG_DECODE,       0b11111111); // decode all digits

    set_register(   1+unit,    inStr.charAt(0));
    
    set_register(   MAX7219_REG_SHUTDOWN, ON);

}

void gspSeven::setIntensity(uint8_t intensity){
    set_register(MAX7219_REG_INTENSITY, intensity & 0x0F);  // turn off display   
}


void gspSeven::clearDisplay()  
{
    set_register(MAX7219_REG_SHUTDOWN, OFF);  // turn off display
    set_register(MAX7219_REG_SCANLIMIT, 7);   // scan limit 8 digits
    set_register(MAX7219_REG_DECODE, 0b11111111); // decode all digits


    for (int i=1;i<9;i++) {
        set_register(   i,    0x0F);

    }

    set_register(MAX7219_REG_SHUTDOWN, ON);   // Turn on display
}


gspSeven::gspSeven(int data, int clock, int chipSelect):
_data(data),_clk(clock),_cs(chipSelect)
{
    pinMode(_data,OUTPUT);  //12
    pinMode(_clk,OUTPUT);   //10
    pinMode(_cs,OUTPUT);    //11
    digitalWrite(_cs, HIGH);
    digitalWrite(_clk, LOW);
    

    //resetAllDisplays();
    
    set_register(OP_SHUTDOWN,OFF);

    //delay(100);

    set_register(OP_DISPLAYTEST,OFF);

    //delay(100);

    set_register(OP_INTENSITY,0X02);

    //clearDisplay();
}

