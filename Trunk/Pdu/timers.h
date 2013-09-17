#ifndef TIMERS_H
#define TIMERS_H

extern void timers_InitVirtualTimers(void);
typedef void (*TPointToHandlerVirtualTimers)(void);
extern void timers_RegVirtualTimer(TPointToHandlerVirtualTimers fpoint);

#endif


