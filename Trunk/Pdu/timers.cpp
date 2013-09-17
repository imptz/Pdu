/*
 Версия модуля 0.2 (06.01.2005)
 модуль содержит код инициализации и обработки прерывания второго таймера,
 настроенного на период 1 милисек. Модуль включает реализацию виртуальных
 таймеров.
 Максимальное число виртуальных таймеров - 10

 - InitVirtualTimers() вызывается для инициализации модул
 - RegVirtualTimer() вызывается для регистрации виртуального таймера, пример: RegVirtualTimer(&TestTimersFunc)
   TestTimersFunc - имя функции основной программы зарегестрированной как обработчик виртуального таймера

*/
#include "main.h"
#include "timers.h"

#define timers_maxNumberTimers 27

TPointToHandlerVirtualTimers timers_VirtualTimers[timers_maxNumberTimers];
unsigned char timers_NumTimers=0;

void timers_InitVirtualTimers(void)
{
 char i;
 for (i=0;i<timers_maxNumberTimers-1;i++) {timers_VirtualTimers[i]=0;}
 timers_NumTimers=0;
 TCCR2=0x0b;
 OCR2=250;
 TIMSK|=0x80;
}

ISR(TIMER2_COMP_vect){
 char i;
 for (i=0;i<timers_NumTimers;i++)
 {
   (*timers_VirtualTimers[i])();
   asm("nop");
 }
 asm("nop");
}

void timers_RegVirtualTimer(TPointToHandlerVirtualTimers fpoint)
{
 if (timers_NumTimers<timers_maxNumberTimers) {timers_VirtualTimers[timers_NumTimers]=fpoint;timers_NumTimers++;}
}
