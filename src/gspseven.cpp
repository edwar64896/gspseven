#include "gspseven.h"



gspGrouped* gspSeven::firstInstance=nullptr;

// ... write a value into a max7219 register 
// See MAX7219 Datasheet, Table 1, page 6
void gspSeven::set_register(byte reg, byte value)  
{
    digitalWrite(_cs, LOW);
    shiftOut(_data, _clk, MSBFIRST, reg);
    shiftOut(_data, _clk, MSBFIRST, value);
    digitalWrite(_cs, HIGH);
}

void gspSeven::resetDisplay()  
{
    set_register(   MAX7219_REG_SHUTDOWN,   OFF);   // turn off display
    set_register(   MAX7219_REG_DISPTEST,   OFF);   // turn off test mode
    set_register(   MAX7219_REG_INTENSITY,  0x0D); // display intensity
    set_register(   MAX7219_REG_SHUTDOWN,   ON);   // turn off display
}

void gspSeven::displayNums(String inStr)  
{
    Serial.println(inStr);
    setDigitValue(   1,    0x0F);
    setDigitValue(   2,    inStr.charAt(6));
    setDigitValue(   3,    inStr.charAt(5));
    setDigitValue(   4,    inStr.charAt(4));
    setDigitValue(   5,    inStr.charAt(2) | DP); // plus decimal point
    setDigitValue(   6,    inStr.charAt(1));
    setDigitValue(   7,    inStr.charAt(0));
    setDigitValue(   8,    0x0F);
}

void gspSeven::displayRtcDate(String inStr)  
{
    setDigitValue(   1,    inStr.charAt(7));
    setDigitValue(   2,    inStr.charAt(6));
    setDigitValue(   3,    inStr.charAt(5));
    setDigitValue(   4,    inStr.charAt(4));
    setDigitValue(   5,    inStr.charAt(3) | DP); // plus decimal point
    setDigitValue(   6,    inStr.charAt(2));
    setDigitValue(   7,    inStr.charAt(1) | DP);
    setDigitValue(   8,    inStr.charAt(0));
}

void gspSeven::displayRtcTime(String inStr)  
{
    setDigitValue(1,inStr.charAt(7));
    setDigitValue(2,inStr.charAt(6));
    setDigitValue(3,0X0A);
    setDigitValue(4,inStr.charAt(4));
    setDigitValue(5,inStr.charAt(3)); // plus decimal point
    setDigitValue(6,0x0A);
    setDigitValue(7,inStr.charAt(1));
    setDigitValue(8,inStr.charAt(0));
}

void gspSeven::displayAllNums(String inStr)  
{
    for (int l=1;l<9;l++) {
        setDigitValue(l,inStr.charAt(9-l));        
    }
}

void gspSeven::setIntensity(uint8_t intensity){
    set_register(MAX7219_REG_INTENSITY, intensity & 0x0F);  // turn off display   
}

void gspSeven::clearDisplay()  
{
    for (int i=1;i<9;i++) {
        setDigitValue(i,0x0F);
    }
}

gspSeven::gspSeven(int data, int clock, int chipSelect):
gspGrouped(),
_data(data),
_clk(clock),
_cs(chipSelect)
{
    pinMode(_data,OUTPUT);  //12
    pinMode(_clk,OUTPUT);   //10
    pinMode(_cs,OUTPUT);    //11

    digitalWrite(_cs, HIGH);
    digitalWrite(_clk, LOW);
    
    set_register(OP_SHUTDOWN,OFF);
    set_register(OP_DISPLAYTEST,OFF);
    set_register(OP_INTENSITY,0X02);

    gspGrouped::startTimer();
}


void gspSeven::render()  
{
    set_register(   MAX7219_REG_SHUTDOWN,     OFF);  // turn off display
    set_register(   MAX7219_REG_SCANLIMIT,    7);   // scan limit 8 digits
    set_register(   MAX7219_REG_DECODE,       0b11111111); // decode all digits

    for (int i=1;i<9;i++) {
        if (_flashDigit[i-1]) {
            if (gspGrouped::_flashState) {
                set_register(   i,    _displayDigits[i-1]);
            } else {
                set_register(   i,    0x0F);
            }
        } else {
            if (_updateDigits[i-1]) {
                set_register(   i,    _displayDigits[i-1]);  
                _updateDigits[i-1]=false;             
            }
        }
    }

    set_register(   MAX7219_REG_SHUTDOWN, ON);
}

bool fs=true;

bool gspSeven::check() {

    if (fs!=gspGrouped::_flashState) {
        fs = gspGrouped::_flashState;
    }
    
    render();
    return true; //keep going with other displays.
}

