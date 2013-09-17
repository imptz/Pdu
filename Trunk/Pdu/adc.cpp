#include "main.h" 
#include "adc.h"
#include "timers.h"

TPointToHandlerADCInterrupt MfuncPoint[8];
unsigned char NumChannel=99;
void ADCExec(void);
void ADCTime(void);

void InitADC(void)
{
 for (unsigned char i = 0; i < 8; ++i) 
	MfuncPoint[i] = 0;
	
 NumChannel=99;
 DDRF=0;
 PORTF=0;
 ADMUX=0;
 SFIOR&=0xef;
 ADCSRA=0x8d;
 timers_RegVirtualTimer(&ADCTime);
}

void ADCExec (void)
{
 if (NumChannel!=99)
 {
  do
   if (NumChannel==7) {NumChannel=0;} else {NumChannel++;}
  while (MfuncPoint[NumChannel]==0);
  ADMUX&=0xe0;
  ADMUX|=NumChannel;
  ADCSRA|=0x40;
 }
}

void RegChannel(TPointToHandlerADCInterrupt fpoint,unsigned char channel)
{
 if (channel<8)
  {
   MfuncPoint[channel]=fpoint;
   if (NumChannel==99) {NumChannel=0;ADCExec();}
  }
}

ISR(ADC_vect){
	(*MfuncPoint[NumChannel])();	
}

void ADCTime(void) {ADCExec();}


