#include <reg932.h>
#include "lcd.h"
#include "lcdcmd.h"
#include <stdio.h>
#define forever for(;;)

//Setup LCD Paramaeters
sbit LCD_E = P1^6;
sbit LCD_RW = P1^4;
sbit LCD_RS = P1^5;
sbit BUSY =P2^7;

// Set Ports in the following Manner

void main(){
    LcdInit();
    forever {
    LcdClear();
    forever{LcdWriteString("a");}
    LcdClear();
    LcdWriteString("Works");
    }

}

//Subroutine to tell if LCD is busy
bit isLcdBusy(void) {
    bit retbit;

    retbit = 0;
    // Set Open Drain to Port 2
    P2M1 = 0x00;
    P2M2 = 0x00;
    LCD_RW = 1;
    LCD_RS = 0;
    LCD_E = 1;
    P2 = 0xFF;
    retbit = P2^7;
    LCD_E = 0;
    //Set Push Pull to Port 2
    P2M1 = 0x00;
    P2M2 = 0xFF;
    LCD_RW = 0;
    return (retbit);
}

//Subroutine to tell LCD To Wait
void waitUntilDone(void)
{
    bit retbit = 1;

//Set Port 2 To Open Drain
    P2M1 = 0xFF;
    P2M2 = 0xFF;
    P2 = 0xFF;
    LCD_RW = 1;
    LCD_RS = 0;
    while (retbit == 1)
    {
       LCD_E = 1;
       retbit = BUSY;
       LCD_E = 0;
    } //while retbit == 1

//Set Port 2 To Push Pull
    P2M1 = 0X00;
    P2M2 = 0XFF;
    LCD_RW = 0;
}

// Write a Character to the LCD Display
void LcdWriteChar(unsigned char dval) 
{
    LCD_RW = 0;
    LCD_RS = 1;
    LCD_E  = 1;
    P2 = dval;
    LCD_E  = 0;
    waitUntilDone();
} // LcdWriteChar()

/*
 * Initialize the LCD.
 */
void LcdWriteCmd(unsigned char cmd)
{
    LCD_RW = 0;
    LCD_RS = 0;
    LCD_E = 1;
    P2 = cmd;
    LCD_E = 0;
    waitUntilDone();
} // LcdWriteCmd

void LcdWriteString(unsigned char *str)
{
    while (*str != '\0')
    {
        LcdWriteChar(*str);
        ++str;
    }
} // LcdWriteString

void LcdInit(void)
{
    P2M1 = 0xFF;    // enable port 1 digital inputs, needed for BUSY.
    P2M2 = 0x00;
    LCD_RW = 0;
    LCD_RS = 0;
    LCD_E  = 0;

    LcdWriteCmd(0x30);
    LcdWriteCmd(0x30);
    LcdWriteCmd(0x30);
    LcdWriteCmd(LCD_SETIFLEN | LCD_SETIFLEN_N | LCD_SETIFLEN_DL); 

    // should be initialized here.

    LcdWriteCmd(LCD_DISPEN); // display off, cursor off, no blink
    LcdWriteCmd(LCD_CLEAR);
    LcdWriteCmd(LCD_DISPEN | LCD_DISPEN_DISP); // | LCD_DISPEN_CURSOR);
    LcdWriteCmd(LCD_MOVEDIR | LCD_MOVEDIR_ID); 
} // LcdInit

void LcdClear(void)
{
    LcdWriteCmd(LCD_CLEAR);
} // LcdClear()

void LcdWriteCGRAM(unsigned char addr, unsigned char pattern)
{
    // First, move the cursor into the CGRAM area:
    LcdWriteCmd(LCD_MOVERAM | addr);
    // Then write the pattern to that location:
    LcdWriteChar(pattern);
} // LcdWriteCGRAM()) 