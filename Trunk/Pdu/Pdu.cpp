#include <avr/io.h>

#include "main.h"
#include "timers.h"
#include "lcd.h"
#include "general.h"
//#include "keyboard.h"
#include "NetDriver_rs485.h"
#include "adc.h"
#include "ls.h"
#include "key.h"

void initPorts();

int qqq=0;
unsigned char f_avar[3];

void initPorts(void){
	asm volatile("cli\n\t"::);
	PORTA=0x00;
	DDRA=0x00;
	PORTB=0x00;
	DDRB=0x00;
	PORTC=0x00;
	DDRC=0x00;
	PORTD=0x00;
	DDRD=0x00;
	PORTE=0;
	DDRE=0;
	PORTF=0x00;
	DDRF=0x00;
	PORTG=0x00;
	DDRG=0x00;
	ASSR=0x00;
	TCCR0=0x00;
	TCNT0=0x00;
	OCR0=0x00;
	TCCR2=0x00;
	TCNT2=0x00;
	OCR2=0x00;
	TIMSK=0x00;
	ETIMSK=0x00;
	UCSR0A=0x00;
	UCSR0B=0x0;
	UCSR0C=0x00;
	UBRR0H=0x00;
	UBRR0L=0x00;
	ACSR=0x00;
	SFIOR=0x00;
	f_avar[0]=0;f_avar[1]=0;f_avar[2]=0;

	asm volatile("sei\n\t"::);
	WDTCR|=(1<<4)|(1<<3);
	WDTCR=0x0f;
	asm volatile("wdr\n\t"::);

	timers_InitVirtualTimers();
	initDelay();
	lcd_InitLCD();
//	keyboard_Init();
	rs485_InitNetwork();
	ls_Init();
	Key::init();
}

int main(void){
	asm volatile("cli\n\t"::);
	
	MCUCSR |= 0x80;
	
	asm volatile("nop\n\t"
	"nop\n\t"
	"nop\n\t"
	"nop\n\t"
	"nop\n\t"
	"nop\n\t"
	"nop\n\t"
	"nop\n\t"
	"nop\n\t"
	"nop\n\t"::);
	
	initPorts();
	while (1){
		ls_main();
		asm volatile("wdr\n\t"::);
	};
	
	return 0;
}
