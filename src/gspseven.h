#ifndef GSP_SEVEN
#define GSP_SEVEN

#include <Arduino.h>
#include <gspgrouped.h>
#include <avr/pgmspace.h>

const static byte charTable [] PROGMEM  = {
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B10000000,B00000001,B10000000,B00000000,
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000,
    B01111111,B01111011,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,B00000000,
    B00110111,B00000000,B00000000,B00000000,B00001110,B00000000,B00000000,B00000000,
    B01100111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00001000,
    B00000000,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,B00000000,
    B00110111,B00000000,B00000000,B00000000,B00001110,B00000000,B00010101,B00011101,
    B01100111,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
    B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};

const static byte fontB [] PROGMEM = {
    B00000000,
    B00000001,
    B00000010,
    B00000011,
    B00000100,
    B00000101,
    B00000110,
    B00000111,
    B00001000,
    B00001001,
    B00001010,
    B00001011,
    B00001100,
    B00001101,
    B00001110,
    B00001111
};

#define OP_NOOP         0
#define OP_DIGIT0       1
#define OP_DIGIT1       2
#define OP_DIGIT2       3
#define OP_DIGIT3       4
#define OP_DIGIT4       5
#define OP_DIGIT5       6
#define OP_DIGIT6       7
#define OP_DIGIT7       8
#define OP_DECODEMODE   9
#define OP_INTENSITY    10
#define OP_SCANLIMIT    11
#define OP_SHUTDOWN     12
#define OP_DISPLAYTEST  15

// enumerate the MAX7219 registers
// See MAX7219 Datasheet, Table 2, page 7
enum {  MAX7219_REG_DECODE    = 0x09,  
        MAX7219_REG_INTENSITY = 0x0A,
        MAX7219_REG_SCANLIMIT = 0x0B,
        MAX7219_REG_SHUTDOWN  = 0x0C,
        MAX7219_REG_DISPTEST  = 0x0F };

// enumerate the SHUTDOWN modes
// See MAX7219 Datasheet, Table 3, page 7
enum  { OFF = 0,  
        ON  = 1 };

const byte DP = 0b10000000;  
const byte C  = 0b01001110;  
const byte F  = 0b01000111;


#define gspSeven_FLASHCOUNT 1048576

class gspSeven : public gspGrouped {

    public:
    
        gspSeven(int data, int clock, int chipSelect);
        ~gspSeven(){};

        void clearDisplay();
        int  getDeviceCount();

        void resetDisplay();
        void displayDigit(uint8_t unit, String inStr)  ;

        void displayNums(String inStr);
        void displayAllNums(String inStr);
        void displayRtcTime(String inStr);
        void displayRtcDate(String inStr);
        void setIntensity(uint8_t intensity);

        void setDigitValue(uint8_t digit, byte value) {
            _displayDigits[digit-1]=value;
            _updateDigits[digit-1]=true;
        }

        static void checkAll() {
            gspGrouped::checkAll(gspSeven::firstInstance);
        }
        
        bool check();
        void flashA(bool state=true){flash(1,state);flash(2,state);flash(3,state);flash(4,state);}
        void flashB(bool state=true){flash(5,state);flash(6,state);flash(7,state);flash(8,state);}

        // flashDigit() or (0) means flash all of them.
        // flashDigit(1...8) means only flash that digit.
        void flash(uint8_t digit=0,bool state=true){
            switch(digit) {
                case 0:
                    flashA(state);
                    flashB(state);
                break;
                default:
                    _flashDigit[digit-1]=state;
                break;
            }
        }

        // means what it says on the tin.
        void stopFlashing(){
                _flashState = false;
                flash(0,false);
            }

  protected:
    void setFirstInstance(gspGrouped * fi) {gspSeven::firstInstance = fi;}
    gspGrouped * getFirstInstance() {return gspSeven::firstInstance;}
     static gspGrouped * firstInstance;

    private:

        int _clk;
        int _data;
        int _cs;

        uint32_t _flashCount=0;
        bool _flashDigit[8]={false,false,false,false,false,false,false,false};
        bool _flashState=false;

        byte _displayDigits[8]={0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F};
        bool _updateDigits[8]={false,false,false,false,false,false,false,false};

        void set_register(byte reg, byte value);
};

#endif
