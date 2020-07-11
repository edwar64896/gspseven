#ifndef GSP_SEVEN
#define GSP_SEVEN

#include "Arduino.h"
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

#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15


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

class gspSeven {

    public:
    
        gspSeven(int data, int clock, int chipSelect, int nDisplays=1);
        ~gspSeven(){};

        //void setScanLimit(int addr, int limit);
        //void setIntensity(int addr, int intensity);
        void clearDisplay(int addr);
        int  getDeviceCount();
        //void shutdown(int addr, bool b);
        void setDigit(int addr, int digit, byte value, boolean dp);
        void setDigits(char*);

        void resetDisplay()  ;
        void displayNums(String dateString)  ;
        //void displayTime(String timeString)  ;
        void resetDisplay(int display)  ;
        void displayNums(int display, String inStr)  ;
        void clearAllDisplays()  ;
        void resetAllDisplays()  ;

    protected:

    private:

        int _clk;
        int _data;
        int _cs;

        int _nDisplays=1;

        //byte spidata[16];
        //byte status[64];

        //void spiTransfer(int addr, volatile byte opcode, volatile byte data);

        void set_register(byte reg, byte value)  ;
        void set_register(uint8_t display, byte reg, byte value)  ;

        void sendRegistersToAll(byte reg, byte value) ; 
};

#endif
