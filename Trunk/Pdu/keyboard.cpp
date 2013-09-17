#include "main.h"
#include "timers.h"
#include "lcd.h"

int keyboard_GetString(int);
void keyboard_TimeFunc(void);
void keyboard_Scan(void);
void keyboard_PutKey(char);
unsigned char keyboard_oldKeyStatea;
unsigned char keyboard_oldKeyStateb;
unsigned char keyboard_oldKeyStatec;
unsigned char keyboard_KeyBuf[10];
unsigned char keyboard_pKeyBuf;
int keyboard_TimeKeyScan;
#define keyboard_cpKeyBuf 10
#define keyboard_cTimeKeyScan 50

unsigned char keyboard_flags;


#define keyboard_SetKeyPort DDRB&=0xc0;PORTB|=0x3f;DDRE&=0x1f;PORTE|=0xe0
#define keyboard_GetPins (PINB&0x3f)
#define keyboard_Row1 DDRE|=0x80;DDRE&=0x9f;PORTE&=0x7f;PORTE|=0x60;asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
#define keyboard_Row2 DDRE|=0x40;DDRE&=0x5f;PORTE&=0xbf;PORTE|=0xa0;asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");
#define keyboard_Row3 DDRE|=0x20;DDRE&=0x3f;PORTE&=0xdf;PORTE|=0xc0;asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");


int keyboard_Init()
{
 keyboard_SetKeyPort;
 keyboard_flags=0;
 timers_RegVirtualTimer(&keyboard_TimeFunc);
 for (keyboard_pKeyBuf = 0; keyboard_pKeyBuf < keyboard_cpKeyBuf; keyboard_pKeyBuf++) 
	keyboard_KeyBuf[keyboard_pKeyBuf]=0;
	
 keyboard_pKeyBuf=0;
 keyboard_oldKeyStatea=keyboard_GetString(1);
 keyboard_oldKeyStateb=keyboard_GetString(2);
 keyboard_oldKeyStatec=keyboard_GetString(3);

 keyboard_TimeKeyScan=0;
 return 0;
}

int keyboard_GetString(int st)
{
 unsigned char i,j; 
 switch (st)
 {
  case 1: keyboard_Row1;break;
  case 2: keyboard_Row2;break;
  case 3: keyboard_Row3;break;
 }
 j = 0;
 i = keyboard_GetPins;
 j|=((i&0x01)<<5);
 j|=((i&0x02)<<3);
 j|=((i&0x04)<<1);
 j|=((i&0x08)>>1);
 j|=((i&0x10)>>3);
 j|=((i&0x20)>>5);
 return j;
}

/*----------------------------------------------------------------------------------*/

void keyboard_TimeFunc(void)
{
 if (keyboard_TimeKeyScan==keyboard_cTimeKeyScan) {keyboard_TimeKeyScan=0;keyboard_Scan();} else keyboard_TimeKeyScan++;
}

void keyboard_PutKey(char n)
  {
   if (keyboard_pKeyBuf!=keyboard_cpKeyBuf) {keyboard_KeyBuf[keyboard_pKeyBuf]=n;keyboard_pKeyBuf++;}
  }

void keyboard_Scan(void)
{
 volatile char t1,t2,t3,t4;
 // line A
 t1=keyboard_GetString(1);
 t2=keyboard_oldKeyStatea ^ t1;
 t3=t1 & t2;
 t4=keyboard_oldKeyStatea & t2;
 if ((t3&0x01)==1) {keyboard_PutKey(0x8b);}
 if ((t3&0x02)==2) {keyboard_PutKey(0x8c);}
 if ((t3&0x04)==4) {keyboard_PutKey(0x8d);}
 if ((t3&0x08)==8) {keyboard_PutKey(0x8e);}
 if ((t3&0x10)==16) {keyboard_PutKey(0x8f);}
 if ((t3&0x20)==32) {keyboard_PutKey(0x90);}
 if ((t4&0x01)==1) {keyboard_PutKey(0x0b);}
 if ((t4&0x02)==2) {keyboard_PutKey(0x0c);}
 if ((t4&0x04)==4) {keyboard_PutKey(0x0d);}
 if ((t4&0x08)==8) {keyboard_PutKey(0x0e);}
 if ((t4&0x10)==16) {keyboard_PutKey(0x0f);}
 if ((t4&0x20)==32) {keyboard_PutKey(0x10);}
 keyboard_oldKeyStatea=t1;
 // line B
 t1=keyboard_GetString(2);
 t2=keyboard_oldKeyStateb ^ t1;
 t3=t1 & t2;
 t4=keyboard_oldKeyStateb & t2;
 if ((t3&0x01)==1) {keyboard_PutKey(0x95);}
 if ((t3&0x02)==2) {keyboard_PutKey(0x96);}
 if ((t3&0x04)==4) {keyboard_PutKey(0x97);}
 if ((t3&0x08)==8) {keyboard_PutKey(0x98);}
 if ((t3&0x10)==16) {keyboard_PutKey(0x99);}
 if ((t3&0x20)==32) {keyboard_PutKey(0x9a);}
 if ((t4&0x01)==1) {keyboard_PutKey(0x15);}
 if ((t4&0x02)==2) {keyboard_PutKey(0x16);}
 if ((t4&0x04)==4) {keyboard_PutKey(0x17);}
 if ((t4&0x08)==8) {keyboard_PutKey(0x18);}
 if ((t4&0x10)==16) {keyboard_PutKey(0x19);}
 if ((t4&0x20)==32) {keyboard_PutKey(0x1a);}
 keyboard_oldKeyStateb=t1;
 // line C
 t1=keyboard_GetString(3);
 t2=keyboard_oldKeyStatec ^ t1;
 t3=t1 & t2;
 t4=keyboard_oldKeyStatec & t2;
 if ((t3&0x01)==1) {keyboard_PutKey(0x9f);}
 if ((t3&0x02)==2) {keyboard_PutKey(0xa0);}
 if ((t3&0x04)==4) {keyboard_PutKey(0xa1);}
 if ((t3&0x08)==8) {keyboard_PutKey(0xa2);}
 if ((t3&0x10)==16) {keyboard_flags&=0x01;}
 if ((t3&0x20)==32) {keyboard_flags&=0x02;}
 if ((t4&0x01)==1) {keyboard_PutKey(0x1f);}
 if ((t4&0x02)==2) {keyboard_PutKey(0x20);}
 if ((t4&0x04)==4) {keyboard_PutKey(0x21);}
 if ((t4&0x08)==8) {keyboard_PutKey(0x22);}
 if ((t4&0x10)==16) {keyboard_flags|=0x02;}
 if ((t4&0x20)==32) {keyboard_flags|=0x01;}
 keyboard_oldKeyStatec=t1;
 DDRE&=0x1f;PORTE|=0xe0;
}

char keyboard_GetKey(void) {if (keyboard_pKeyBuf!=0) {keyboard_pKeyBuf--;return (keyboard_KeyBuf[keyboard_pKeyBuf]);} else return(0);}
char keyboard_TestKey(void) {if (keyboard_pKeyBuf!=0) {return (keyboard_KeyBuf[keyboard_pKeyBuf-1]);} else return(0);}

void keyboard_ClearKeyQueue()
{
 keyboard_pKeyBuf=0;	
}

unsigned char keyboard_GetFlags()
{
 return keyboard_flags;
}

unsigned char keyboard_TestF1()
{
  if ((keyboard_flags&0x02)==0) {return 0;} else {return 1;}
}
unsigned char keyboard_TestF2()
{
  if ((keyboard_flags&0x01)==0) {return 0;} else {return 1;}
}

