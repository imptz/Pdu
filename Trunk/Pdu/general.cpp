#include "main.h"
#include "timers.h"
#include "general.h"


void delay_mks_1000(int time)
{
 int i;int d;
 for (d=0;d<time+1;d++) {for (i=0;i<1050;i++) {asm ("nop");}}
}
void delay_mks_100(int time)
{
 int i;int d;
 for (d=0;d<time+1;d++) {for (i=0;i<85;i++) {asm ("nop");}}
}
void delay_mks_10(int time)
{
 int i;int d;
 for (d=0;d<time+1;d++) {for (i=0;i<6;i++) {asm ("nop");}}
}


void delayFunc(void);
volatile char f_delay;
int mdelay;

void initDelay(void)
{
 mdelay=0;f_delay=0;
 timers_RegVirtualTimer(&delayFunc);
}

void delayFunc(void)
{
 if (mdelay==0) {f_delay=0;} else mdelay--;
}

void delay(int n)
{
 f_delay=1;mdelay=n;while (f_delay){asm("wdr");};
}


