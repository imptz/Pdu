#include <stdarg.h>
#include "main.h"
#include "general.h"
#include "NetDriver_rs485.h"
#include "timers.h"
#include "lcd.h"
#include "ls.h"
#include "listCommand.h"

void rs485_StartSend(void);
void rs485_SendFunc(void);
void rs485_SendControlSum(void);

unsigned char rs485_ReceiveControlSum(void);
void rs485_ControlTimeRecByte(void);
void rs485_PeriodicNetDriverFunc(void);

unsigned char rs485_deviceAddress=0;
TPointer_HandlerRecPackage rs485_pointer_HandlerRecPackage;
TPointer_SetUpPortUartFunc rs485_pointer_SetUpPortUartFunc;
TPointer_RecToSendFunc rs485_pointer_RecToSendFunc;
TPointer_SendToRecFunc rs485_pointer_SendToRecFunc;
#define rs485_SetUpPortUart (*rs485_pointer_SetUpPortUartFunc)();
#define rs485_RecToSend (*rs485_pointer_RecToSendFunc)();UCSR0B|=0x08;UCSR0B&=0xef;
#define rs485_SendToRec (*rs485_pointer_SendToRecFunc)();UCSR0B|=0x10;UCSR0B&=0xfe;

#define FRAMING_ERROR (0x10)
#define DATA_OVERRUN  (0x08)

unsigned char rs485_receiveBuf[21]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char rs485_r_count=0;
unsigned char rs485_rec_control_count=0;  // счетчик контроля времени приема
unsigned char rs485_f_rec_control_count=0;  // флаг контроля времени приема 1 - вкл, 0 - выкл
#define rs485_const_rec_control_count (10) // константа счетчика контроля приема

unsigned char rs485_sendBuf[21]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char rs485_s_count=0;
unsigned char rs485_f_sending=0;

#define rs485_TestCTS (PINE&0x08)
unsigned char rs485_fSndTime;


void rs485_InitNetDriver(char lDeviceAddress,char ubh,char ubl, TPointer_HandlerRecPackage lPointer_HandlerRecPackage,
	 				  	 TPointer_SetUpPortUartFunc lPointer_SetUpPortUartFunc,
						 TPointer_RecToSendFunc lPointer_RecToSendFunc,
						 TPointer_SendToRecFunc lPointer_SendToRecFunc)
{
 rs485_fSndTime=0;
 UCSR0A=0x00;
 UCSR0B=0xd0;
 UCSR0C=0x86;
 UBRR0H=ubh;
 UBRR0L=ubl;
 rs485_deviceAddress=lDeviceAddress;
 rs485_pointer_HandlerRecPackage=lPointer_HandlerRecPackage;
 rs485_pointer_SetUpPortUartFunc=lPointer_SetUpPortUartFunc;
 rs485_pointer_RecToSendFunc=lPointer_RecToSendFunc;
 rs485_pointer_SendToRecFunc=lPointer_SendToRecFunc;
 rs485_SetUpPortUart
 rs485_SendToRec
 timers_RegVirtualTimer(&rs485_PeriodicNetDriverFunc);
}

void rs485_FillPackage(char da,char sa,char com,char npar, ...)
{
 char i;
 va_list par;
 va_start(par,npar);
 rs485_sendBuf[0]=da;
 rs485_sendBuf[1]=sa;
 rs485_sendBuf[2]=com;
 rs485_sendBuf[3]=npar;
 i=4;
 for (i=4;i<(4+npar);i++)
  {
   int p = va_arg(par,int);
   rs485_sendBuf[i]=p;
  }
 rs485_sendBuf[20]=1;
 rs485_StartSend();
 va_end(par);
}

void rs485_FillPackage_1(char da,char sa,char com,char npar, ...)
{
 char i;
 if (rs485_sendBuf[20]==0)
  {
   va_list par;
   va_start(par,npar);
   rs485_sendBuf[0]=da;
   rs485_sendBuf[1]=sa;
   rs485_sendBuf[2]=com;
   rs485_sendBuf[3]=npar;
   i=4;
   for (i=4;i<(4+npar);i++)
    {
     int p = va_arg(par,int);
     rs485_sendBuf[i]=p;
    }
   rs485_sendBuf[20]=1;
   va_end(par);
  }
}

void rs485_SendControlSum(void)
{
 char i,j,k;
 j=rs485_sendBuf[3]+4;
 k=0;
 for (i=0;i<j;i++) {k+=rs485_sendBuf[i];}
 rs485_sendBuf[j]=k;
}

void rs485_StartSend(void)
{
 if ((rs485_f_sending==0)&&(rs485_sendBuf[20]!=0))
  {
   rs485_f_sending=1;
   rs485_SendControlSum();
   rs485_s_count=0;
   rs485_SendFunc();
  }
}

void rs485_SendFunc(void)
{
 if (rs485_f_sending==1)
 {
  if ((rs485_s_count!=(rs485_sendBuf[3]+5)) && (rs485_s_count<20))
  {
   rs485_RecToSend delay_mks_10(10);UDR0=rs485_sendBuf[rs485_s_count];rs485_s_count++;
  }
  else {rs485_SendToRec rs485_sendBuf[20]=0;rs485_f_sending=0;}
 }
}

ISR(USART0_TX_vect){
	rs485_SendFunc();
}

/////////////////////////////////////////////////////////////////////////////////////

unsigned char rs485_ReceiveControlSum(void)
{
 char i,j,k;
 j=rs485_receiveBuf[3]+4;if (j>19) {return (0);}
 k=0;
 for (i=0;i<j;i++) {k+=rs485_receiveBuf[i];}
 if (k==rs485_receiveBuf[j]) {return (1);} else {return (0);}
}

ISR(USART0_RX_vect){
 char status;
 status=UCSR0A;
 rs485_rec_control_count=0;
 if ((status & ((FRAMING_ERROR)|(DATA_OVERRUN)))==0)
  {
   status=UDR0;
   if (rs485_r_count==0)
   {
     if (status==rs485_deviceAddress)
     {
       rs485_receiveBuf[rs485_r_count]=status;
       rs485_r_count=1;
     }
   }
   else
    {
     rs485_receiveBuf[rs485_r_count]=status;
     rs485_r_count++;
     if (rs485_r_count>=5)
      if ((rs485_r_count==5+rs485_receiveBuf[3])||(rs485_r_count==21))
       {
        rs485_r_count=0;
        if (rs485_ReceiveControlSum()==1) {if (*rs485_pointer_HandlerRecPackage!=0) {(*rs485_pointer_HandlerRecPackage)();}}
       }
    }
  } else {status=UDR0;}
}

void rs485_ControlTimeRecByte(void)
{
 if (rs485_rec_control_count==(rs485_const_rec_control_count-1))
  {
   rs485_rec_control_count=rs485_const_rec_control_count;
   rs485_r_count=0;
  }
 else if (rs485_rec_control_count!=rs485_const_rec_control_count) {rs485_rec_control_count++;}
}

void rs485_PeriodicNetDriverFunc(void)  // период вызова ~1 ms
{
 rs485_ControlTimeRecByte();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void rs485_RecPackageFunc(void);
void rs485_SetUpPortUartFunc(void);
void rs485_RecToSendFunc(void);
void rs485_SendToRecFunc(void);

#define SG_COUNT 40

unsigned char rs485_lsAddress;
unsigned char rs485_sg[20*SG_COUNT];
unsigned int rs485_sgCounter;
unsigned char rs485_nRequest;

void rs485_InitNetwork(void)
{
 rs485_nRequest=0;
 DDRF=0x00;PORTF=0xff;
 for (int i = 0; i < 100; ++i)
 {
	PORTF=0xff;
	asm volatile("wdr\n\t"::);
 }	
 asm("nop");
 rs485_sgCounter=0;
 rs485_lsAddress=((~PINF) & 0x07) + 33;
 //lcd_PutNum(18, rs485_lsAddress);
 rs485_InitNetDriver(rs485_lsAddress,0,16,&rs485_RecPackageFunc,&rs485_SetUpPortUartFunc,&rs485_RecToSendFunc,&rs485_SendToRecFunc);
}

int rs485_debugCounter=0;
int rs485_debugCounter1=0;


void rs485_RecPackageFunc(void)
{
 unsigned int i;
 static int gateNum = 0;

 switch (rs485_receiveBuf[2])
  {
   case testNCDevice:
    rs485_FillPackage(rs485_receiveBuf[1],rs485_lsAddress,120,1,5);
    break;
#ifdef ELR           
   case InfoFromLS:
    elrInfo(rs485_receiveBuf[1]);
    break;
#endif    
   case answerLsAll: 
     if (rs485_receiveBuf[4] == reqSetMode)
     {
      SetMode();
     }
     else
     if ((rs485_receiveBuf[4] == requestInfoFromLS) || (rs485_receiveBuf[4] == requestInfoFromZatvor))
     {
#ifndef ELR       
      ls_ProcessInfo(rs485_receiveBuf[1]);
#endif      
     }
     break;
   case answerLsList:
    ls_SetLsPresence(&rs485_receiveBuf[4]);
    break;
   case permitTransmfer:
    if (rs485_f_sending==0)
    {
     asm("cli");
     if (rs485_sgCounter!=0)
     {
      rs485_FillPackage(rs485_sg[0],rs485_sg[1],rs485_sg[2],
                        rs485_sg[3],rs485_sg[4],rs485_sg[5],
                        rs485_sg[6],rs485_sg[7],rs485_sg[8],
                        rs485_sg[9],rs485_sg[10],rs485_sg[11],
                        rs485_sg[12],rs485_sg[13],rs485_sg[14],
                        rs485_sg[15],rs485_sg[16],rs485_sg[17],
                        rs485_sg[18],rs485_sg[19]);
      for (i=20;i<rs485_sgCounter*20;i++) {rs485_sg[i-20]=rs485_sg[i];}
      rs485_sgCounter--;
     }
     else
     {
      if (rs485_nRequest<4)
      {
       if (ls_currentLs!=0)
       {
	rs485_FillPackage(ls_currentLs,rs485_lsAddress,requestInfoFromLS,0);
       }
       rs485_nRequest++;
      }
      else
       if (rs485_nRequest<5)
       {
        if (ls_currentLs!=0)
        {
 	 rs485_FillPackage(ls_currentLs,rs485_lsAddress,requestInfoFromZatvor,1,gateNum);
	  if (gateNum == 0)
		gateNum = 1;
	  else
		gateNum = 0;
        }
        rs485_nRequest++;
       }
       else
       {
        rs485_FillPackage(33,rs485_lsAddress,requestLsList,0);
        rs485_nRequest=0;
       }
     }
     asm("sei");
    }
    break;
  }
}

void rs485_FillPackageG(char da,char sa,char com,char npar, ...)
{
 unsigned int i;
 va_list par;
 va_start(par,npar);
 asm("cli");

 if (rs485_sgCounter<SG_COUNT)
 {
  rs485_sg[rs485_sgCounter*20+0]=da;
  rs485_sg[rs485_sgCounter*20+1]=sa;
  rs485_sg[rs485_sgCounter*20+2]=com;
  rs485_sg[rs485_sgCounter*20+3]=npar;
  i=4;
  for (i=4;i<(4+npar);i++)
   {
    int p = va_arg(par,int);
    rs485_sg[rs485_sgCounter*20+i]=p;
   }
  rs485_sgCounter++;
 }

 if (rs485_sgCounter==SG_COUNT)
 {
  if ( (rs485_sg[(rs485_sgCounter-1)*20+2]!=stopV)&&
       (rs485_sg[(rs485_sgCounter-1)*20+2]!=stopG)&&
       (rs485_sg[(rs485_sgCounter-1)*20+2]!=stopN)
     )
  {
    rs485_sgCounter--;
  }
 }

 asm("sei");
 va_end(par);
}


void rs485_SetUpPortUartFunc(void) { DDRE|=0x0e;PORTE&=0xf1;}
void rs485_RecToSendFunc(void) {PORTE|=0x0c;}
void rs485_SendToRecFunc(void) {PORTE&=0xf3;}


























