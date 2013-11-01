// управление КЛАПАНОМ затвора отключено


#include "main.h"
#include "ls.h"
//#include "keyboard.h"
#include "key.h"
#include "lcd.h"
#include "NetDriver_rs485.h"
#include "listCommand.h"
#include "timers.h"

unsigned int f2Pr = 0;
unsigned int PrNap = 0;

void ls_ChangeModeFunc(unsigned char);
void ls_ChangeModeDisplay();
void ls_PrintDisplay();

ls_TLs ls_ls[33];
unsigned char ls_currentLs;

unsigned char ls_NoSetLS;
#define ls_SET_NoSetLS ls_NoSetLS++
#define ls_CLEAR_NoSetLS {if (ls_NoSetLS!=0) {ls_NoSetLS--;}}

void ls_ChangeLs();
void ls_keyMode_0();
void ls_keyMode_1();
void ls_keyMode_2();
void ls_keyMode_3();
void ls_keyMode_4();
int ls_keyMode_3_time;
int ls_keyMode_4_time;
void ls_TimeFunc();

unsigned char ls_ProcessInfoCount;
#define ls_ProcessInfoCountMax 1

void ls_Init()
{
 unsigned char i;
 ls_keyMode_3_time = 0;
 ls_keyMode_4_time = 0;
#ifdef BEZ_SK
 ls_currentLs=1;
#else
 ls_currentLs=0;
#endif 
 ls_NoSetLS=0;
#ifdef EngL
 lcd_PutString(0,"LS N: ");
#else
 lcd_PutString(0,"ЛС N: ");
#endif
 lcd_PutNum2(5,0);
 for (i=0;i<33;i++)
 {
  ls_ls[i].presence=0;
  ls_ls[i].current=0;
  ls_ls[i].pressure=0;
  ls_ls[i].nProg=0;
  ls_ls[i].ps=0;
  ls_ls[i].speed=7;
  ls_ls[i].ustFS=0;
  ls_ls[i].alarm=0;
  ls_ls[i].modeDisplay=0;
  ls_ls[i].modeFunc=0;
  ls_ls[i].zatvor=0;
  ls_ls[i].zatvor1=0;
  ls_ls[i].step=0;
 }
 ls_ProcessInfoCount=0;
 timers_RegVirtualTimer(&ls_TimeFunc);
}

void ls_SetLsPresence(unsigned char *list)
{
 ls_ls[32].presence=(((*list)&0x80)>>7);
 ls_ls[31].presence=(((*list)&0x40)>>6);
 ls_ls[30].presence=(((*list)&0x20)>>5);
 ls_ls[29].presence=(((*list)&0x10)>>4);
 ls_ls[28].presence=(((*list)&0x08)>>3);
 ls_ls[27].presence=(((*list)&0x04)>>2);
 ls_ls[26].presence=(((*list)&0x02)>>1);
 ls_ls[25].presence=(((*list)&0x01));
 ls_ls[24].presence=((*(list+1)&0x80)>>7);
 ls_ls[23].presence=((*(list+1)&0x40)>>6);
 ls_ls[22].presence=((*(list+1)&0x20)>>5);
 ls_ls[21].presence=((*(list+1)&0x10)>>4);
 ls_ls[20].presence=((*(list+1)&0x08)>>3);
 ls_ls[19].presence=((*(list+1)&0x04)>>2);
 ls_ls[18].presence=((*(list+1)&0x02)>>1);
 ls_ls[17].presence=((*(list+1)&0x01));
 ls_ls[16].presence=((*(list+2)&0x80)>>7);
 ls_ls[15].presence=((*(list+2)&0x40)>>6);
 ls_ls[14].presence=((*(list+2)&0x20)>>5);
 ls_ls[13].presence=((*(list+2)&0x10)>>4);
 ls_ls[12].presence=((*(list+2)&0x08)>>3);
 ls_ls[11].presence=((*(list+2)&0x04)>>2);
 ls_ls[10].presence=((*(list+2)&0x02)>>1);
 ls_ls[9].presence=((*(list+2)&0x01));
 ls_ls[8].presence=((*(list+3)&0x80)>>7);
 ls_ls[7].presence=((*(list+3)&0x40)>>6);
 ls_ls[6].presence=((*(list+3)&0x20)>>5);
 ls_ls[5].presence=((*(list+3)&0x10)>>4);
 ls_ls[4].presence=((*(list+3)&0x08)>>3);
 ls_ls[3].presence=((*(list+3)&0x04)>>2);
 ls_ls[2].presence=((*(list+3)&0x02)>>1);
 ls_ls[1].presence=((*(list+3)&0x01));

 ls_ls[32].presence|=((*(list+5)&0x80)>>7);
 ls_ls[31].presence|=((*(list+5)&0x40)>>6);
 ls_ls[30].presence|=((*(list+5)&0x20)>>5);
 ls_ls[29].presence|=((*(list+5)&0x10)>>4);
 ls_ls[28].presence|=((*(list+5)&0x08)>>3);
 ls_ls[27].presence|=((*(list+5)&0x04)>>2);
 ls_ls[26].presence|=((*(list+5)&0x02)>>1);
 ls_ls[25].presence|=((*(list+5)&0x01));
 ls_ls[24].presence|=((*(list+6)&0x80)>>7);
 ls_ls[23].presence|=((*(list+6)&0x40)>>6);
 ls_ls[22].presence|=((*(list+6)&0x20)>>5);
 ls_ls[21].presence|=((*(list+6)&0x10)>>4);
 ls_ls[20].presence|=((*(list+6)&0x08)>>3);
 ls_ls[19].presence|=((*(list+6)&0x04)>>2);
 ls_ls[18].presence|=((*(list+6)&0x02)>>1);
 ls_ls[17].presence|=((*(list+6)&0x01));
 ls_ls[16].presence|=((*(list+7)&0x80)>>7);
 ls_ls[15].presence|=((*(list+7)&0x40)>>6);
 ls_ls[14].presence|=((*(list+7)&0x20)>>5);
 ls_ls[13].presence|=((*(list+7)&0x10)>>4);
 ls_ls[12].presence|=((*(list+7)&0x08)>>3);
 ls_ls[11].presence|=((*(list+7)&0x04)>>2);
 ls_ls[10].presence|=((*(list+7)&0x02)>>1);
 ls_ls[9].presence|=((*(list+7)&0x01));
 ls_ls[8].presence|=((*(list+8)&0x80)>>7);
 ls_ls[7].presence|=((*(list+8)&0x40)>>6);
 ls_ls[6].presence|=((*(list+8)&0x20)>>5);
 ls_ls[5].presence|=((*(list+8)&0x10)>>4);
 ls_ls[4].presence|=((*(list+8)&0x08)>>3);
 ls_ls[3].presence|=((*(list+8)&0x04)>>2);
 ls_ls[2].presence|=((*(list+8)&0x02)>>1);
 ls_ls[1].presence|=((*(list+8)&0x01));

 if ((ls_currentLs!=0)&&(ls_ls[ls_currentLs].presence==0)) {ls_ChangeLs();}
}

void ls_ChangeLs()
{
 do
 {
  if (ls_currentLs<32) {ls_currentLs++;}
  else
  {
   ls_currentLs=0;
   lcd_PutString(0,sEmpty);
#ifdef EngL
 lcd_PutString(0,"LS N: ");
#else
 lcd_PutString(0,"ЛС N: ");
#endif
  }
 }
 while ((ls_ls[ls_currentLs].presence==0)&&(ls_currentLs!=0));
 lcd_PutNum2(5,ls_currentLs);
 if (ls_currentLs==0)
 {
  lcd_PutString(16,"                ");
  lcd_PutString(32,"                ");
  lcd_PutString(48,"                ");
 }
#ifndef ELR 
 else 
   ls_PrintDisplay();
#endif 
}

void ls_main()
{
	  if (f2Pr == 2000)
	  {
	    f2Pr = 3000;
		if (PrNap == 1)
		{
			rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL,1,1);ls_SET_NoSetLS;// в рабочее
		}
		else
		{
		   rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL,1,2);ls_SET_NoSetLS;// в дежурное
		}
	  }
	  else
	  {

 switch (ls_ls[ls_currentLs].modeFunc)
 {
  case 0: ls_keyMode_0();break;
  case 1: ls_keyMode_1();break;
  case 2: ls_keyMode_2();break;
  case 3: ls_keyMode_3();break;
  case 4: ls_keyMode_4();break;
 }
// ls_PrintDisplay();
	  }
}

void ls_ChangeModeDisplay()
{
 if (ls_ls[ls_currentLs].modeDisplay==1){
	 ls_ls[ls_currentLs].modeDisplay=7;
 }else{
	 if (ls_ls[ls_currentLs].modeDisplay==7){
		 ls_ls[ls_currentLs].modeDisplay=2;
	 }else{
			if (ls_ls[ls_currentLs].modeDisplay<5) {ls_ls[ls_currentLs].modeDisplay++;}
			else if (ls_ls[ls_currentLs].modeDisplay==5) {ls_ls[ls_currentLs].modeDisplay=0;}
	 }		 		 	 
 }
 ls_PrintDisplay();
}

void ls_PrintDisplay()
{
 if (ls_currentLs!=0)
 {
  switch (ls_ls[ls_currentLs].modeDisplay)
  {
   case 0: 
#ifdef EngL     
     lcd_PutString(32,"Current:      A ");
     lcd_PutNumf(41,ls_ls[ls_currentLs].current);break;
#else     
     lcd_PutString(32,"Ток:      A     ");
     lcd_PutNumf(37,ls_ls[ls_currentLs].current);break;
#endif
   case 1:
#ifdef EngL
	   lcd_PutString(32,"Gate:           ");
		switch (ls_ls[ls_currentLs].zatvor)
		{
			case 0: lcd_PutString(41,"Closed  ");break;
			case 1: lcd_PutString(41,"Opened  ");break;
			case 2: lcd_PutString(41,"Work    ");break;
			case 3: lcd_PutString(41,"Crash   ");break;
		}
		break;
#else
	   lcd_PutString(32,"Затвор 1:       ");
		switch (ls_ls[ls_currentLs].zatvor)
		{
			case 0: lcd_PutString(41,"закрыт  ");break;
			case 1: lcd_PutString(41,"открыт  ");break;
			case 2: lcd_PutString(41,"работа  ");break;
			case 3: lcd_PutString(41,"авария  ");break;
		}
		break;
#endif
   case 7:
#ifdef EngL
	   lcd_PutString(32,"Gate 2:         ");
		switch (ls_ls[ls_currentLs].zatvor1)
		{
			case 0: lcd_PutString(41,"Closed  ");break;
			case 1: lcd_PutString(41,"Opened  ");break;
			case 2: lcd_PutString(41,"Work    ");break;
			case 3: lcd_PutString(41,"Crash   ");break;
		}
		break;
#else
	   lcd_PutString(32,"Затвор 2:       ");
		switch (ls_ls[ls_currentLs].zatvor1)
		{
			case 0: lcd_PutString(41,"закрыт  ");break;
			case 1: lcd_PutString(41,"открыт  ");break;
			case 2: lcd_PutString(41,"работа  ");break;
			case 3: lcd_PutString(41,"авария  ");break;
		}
		break;
#endif
   case 2:
#ifdef EngL     
    lcd_PutString(32,"Valve:          ");
    switch (ls_ls[ls_currentLs].valve)
    {
     case 0: lcd_PutString(39,"Closed    ");break;
     case 1: lcd_PutString(39,"Opened    ");break;
    } 
    break;
#else
    lcd_PutString(32,"Клапан:         ");
    switch (ls_ls[ls_currentLs].valve)
    {
     case 0: lcd_PutString(39,"закрыт    ");break;
     case 1: lcd_PutString(39,"открыт    ");break;
    } 
    break;
#endif    
#ifdef EngL     
   case 3: lcd_PutString(32,"Press:      Atm ");lcd_PutNumf(39,ls_ls[ls_currentLs].pressure);break;
   case 4: lcd_PutString(32,"Prog num:       ");lcd_PutNum1(42,ls_ls[ls_currentLs].nProg);break;
   case 5: lcd_PutString(32,"Speed:          ");lcd_PutNum1(42,ls_ls[ls_currentLs].speed+1);break;
   case 6: lcd_PutString(32,"Step:           ");lcd_PutNum(42,ls_ls[ls_currentLs].step);break;
#else
   case 3: lcd_PutString(32,"Дав:        Атм ");lcd_PutNumf(37,ls_ls[ls_currentLs].pressure);break;
   case 4: lcd_PutString(32,"Ном прог:       ");lcd_PutNum1(42,ls_ls[ls_currentLs].nProg);break;
   case 5: lcd_PutString(32,"Скорость:       ");lcd_PutNum1(42,ls_ls[ls_currentLs].speed+1);break;
   case 6: lcd_PutString(32,"Шаг:            ");lcd_PutNum(42,ls_ls[ls_currentLs].step);break;
#endif   
  }
  switch (ls_ls[ls_currentLs].modeFunc)
  {
   case 0:
#ifdef EngL     
    if (ls_ls[ls_currentLs].ps==1) {lcd_PutString(48,"    Scanning    ");}
#else
    if (ls_ls[ls_currentLs].ps==1) {lcd_PutString(48,"  Сканирование  ");}
#endif    
    else {ls_ls[ls_currentLs].ps=0;lcd_PutString(48,"                ");}
    break;
#ifdef EngL     
   case 1: lcd_PutString(48,"Program install ");break;
   case 2: lcd_PutString(48,"   Settings     ");break;
   case 3: lcd_PutString(48,"  Open a gate?  ");break;
   case 4: lcd_PutString(48,"  Open a gate?  ");break;
#else
   case 1: lcd_PutString(48,"Устан. программы");break;
   case 2: lcd_PutString(48,"   Настройки    ");break;
   case 3: lcd_PutString(48,"Открыть затвор1?");break;
   case 4: lcd_PutString(48,"Открыть затвор2?");break;
#endif    
  }
 }
}

void ls_ChangeModeFunc(unsigned char nmode)
{
 switch (nmode)
 {
  case 0: ls_CLEAR_NoSetLS;ls_ls[ls_currentLs].modeDisplay=0;lcd_PutString(16,"                ");break;
  case 1: ls_SET_NoSetLS;ls_ls[ls_currentLs].modeDisplay=6;break;
  case 2: ls_SET_NoSetLS;break;
  case 3: ls_SET_NoSetLS;ls_keyMode_3_time = 10000;break;
  case 4: ls_SET_NoSetLS;ls_keyMode_4_time = 10000;break;
 }
 ls_ls[ls_currentLs].modeFunc=nmode;
}

void ls_keyMode_0()
{
 switch (keyboard_GetKey())
 {
#ifdef FIRE_KEY   
  case bVid_press:
   rs485_FillPackageG(ls_currentLs,rs485_lsAddress,FIRE,10,0,45,1,0x3b,1,0x3b,0,45,0,2);
   break;
#endif
#ifndef ELR
  case bZap_press:
   if (ls_ls[ls_currentLs].ps==0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setProgPoint1,0);ls_ChangeModeFunc(1);}
   break;
  case bNProg_press:
   if (keyboard_TestF2()==1)
   {
    rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,6);
   }
   else
   {
    if (ls_ls[ls_currentLs].nProg<7) {ls_ls[ls_currentLs].nProg++;} else {ls_ls[ls_currentLs].nProg=0;}
    ls_PrintDisplay();
   }
   break;
  case bPlus_press:
   if (keyboard_TestF2()==1)
   {
    ls_ChangeModeDisplay();
   }
   else
   {
    if (ls_ls[ls_currentLs].speed<7) {ls_ls[ls_currentLs].speed++;}
    if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setSpeed,1,ls_ls[ls_currentLs].speed);}
   }
   break;
  case bMinus_press:
   if (ls_ls[ls_currentLs].speed>0) {ls_ls[ls_currentLs].speed--;}
   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setSpeed,1,ls_ls[ls_currentLs].speed);}
   break;
  case bZtOpen_press: 
	if (ls_currentLs!=0){
		if (keyboard_TestF1()==1){
			//if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,KLAPAN,1,1);}
			ls_ChangeModeFunc(3);
		}else{ 
			if (keyboard_TestF2()==1){
				ls_ChangeModeFunc(4);
			}
		}			
   }		
   break; 
  case bZtClose_press: 
   if (keyboard_TestF1()==1)
   {
    //if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,KLAPAN,1,0);}
		if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorClose,1,0);}	   
	}  
   else 
   if (keyboard_TestF2()==1)
   {
     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorClose,1,1);}
   }
   break;
  case bZtStop_press:  
	if (ls_currentLs!=0){
		if (keyboard_TestF1()==1){
			rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorStop,1,0);
		}else{
			if (keyboard_TestF2()==1){
				rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorStop,1,1);
			}				
		}			
	}
	break;
  case bNn_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,nN,0);ls_SET_NoSetLS;}break;
  case bNn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);ls_CLEAR_NoSetLS;}break;
  case bWn_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,wN,0);ls_SET_NoSetLS;}break;
  case bWn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);ls_CLEAR_NoSetLS;}break;
#endif
 case bUp_press:     
	  if (ls_currentLs!=0) 
	  {
		  if (keyboard_TestF1()==1)
		  {
			  f2Pr = 1;
			  PrNap = 1;
		  }
		  else
		  {
			  rs485_FillPackageG(ls_currentLs,rs485_lsAddress,up,0);ls_SET_NoSetLS;
		  }
	  }
	  break;
  case bUp_release:   
	  if (ls_currentLs!=0) 
	  {
		  if (PrNap == 0)
		  {
			rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);
			ls_CLEAR_NoSetLS;
		  }
		  else
		  {
			  PrNap = 0;
			  f2Pr = 0;
			rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL,1, 0);ls_CLEAR_NoSetLS; // стоп поворотный
		  }
	  }
	  break;
  case bDown_press:   
	  if (ls_currentLs!=0) 
	  {
		  if (keyboard_TestF1()==1)
		  {
			  f2Pr = 1;
			  PrNap = 2;
		  }
		  else
		  {
			  rs485_FillPackageG(ls_currentLs,rs485_lsAddress,down,0);ls_SET_NoSetLS;
		  }
	  }
	  break;
  case bDown_release: 
	  if (ls_currentLs!=0) 
	  {
		  if (PrNap == 0)
		  {
			rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);
			ls_CLEAR_NoSetLS;
		  }
		  else
		  {
			  PrNap = 0;
			  f2Pr = 0;
			rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL,1, 0);ls_CLEAR_NoSetLS; // стоп поворотный
		  }
	  }
	  break;
  case bLeft_press:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,left,0);ls_SET_NoSetLS;}break;
  case bLeft_release: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);ls_CLEAR_NoSetLS;}break;
  case bRight_press:  if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,right,0);ls_SET_NoSetLS;}break;
  case bRight_release:if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);ls_CLEAR_NoSetLS;}break;
#ifndef BEZ_SK     
  case bLsN_press:    if (ls_NoSetLS==0) {ls_ChangeLs();}break;
#endif
#ifndef ELR
 case bPS_press:
   if (ls_currentLs!=0)
   {
     if (ls_ls[ls_currentLs].ps==0) 
     {
      rs485_FillPackageG(ls_currentLs,rs485_lsAddress,startProg,1,ls_ls[ls_currentLs].nProg);
     }
     else {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopProg,0);}
   }
   break;
#endif   
 }
 if (keyboard_TestF1()==1)
  {
   if (ls_ls[ls_currentLs].alarm==1)
   {
    f_avar[0]=0;f_avar[1]=0;f_avar[2]=0;Beep_off;lcd_PutString(16,"                ");ls_ls[ls_currentLs].alarm=0;
   }
  }
}

void ls_keyMode_1()
{
 switch (keyboard_GetKey())
 {
  case bZap_press:
   rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setProgPoint2,2,ls_ls[ls_currentLs].step,ls_ls[ls_currentLs].nProg);
   ls_ChangeModeFunc(0);
   break;

  case bPlus_press:  
   if (ls_ls[ls_currentLs].step<90) {ls_ls[ls_currentLs].step++;}
   ls_PrintDisplay();
   break;
  case bMinus_press: 
   if (ls_ls[ls_currentLs].step>0) {ls_ls[ls_currentLs].step--;}
   ls_PrintDisplay();
   break;
  case bZtOpen_press: 
   if (keyboard_TestF1()==1)
   {
    //if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,KLAPAN,1,1);}
   }  
   else 
   if (keyboard_TestF1()==1)
   {
     //ls_ChangeModeFunc(3);
   }
    break; 
  case bZtClose_press: 
   if (keyboard_TestF1()==1)
   {
    //if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,KLAPAN,1,0);}
   }  
   else 
   if (keyboard_TestF1()==1)
   {
     //if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorClose,0);}
   }
   break;
  //case bZtStop_press: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorStop,0);}break;
  case bNn_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,nN,0);}break;
  case bNn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);}break;
  case bWn_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,wN,0);}break;
  case bWn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);}break;
  case bUp_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,up,0);}break;
  case bUp_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);}break;
  case bDown_press:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,down,0);}break;
  case bDown_release: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);}break;
  case bLeft_press:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,left,0);}break;
  case bLeft_release: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);}break;
  case bRight_press:  if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,right,0);}break;
  case bRight_release:if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);}break;
 }
 if (keyboard_TestF1()==1)
  {
   if (ls_ls[ls_currentLs].alarm==1)
   {
    f_avar[0]=0;f_avar[1]=0;f_avar[2]=0;Beep_off;lcd_PutString(16,"                ");ls_ls[ls_currentLs].alarm=0;
   }
  }
}
void ls_keyMode_2()
{
 switch (keyboard_GetKey())
 {
	case bVid_press:
		if (keyboard_TestF1()==1){
			rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1, 16);
		}else
			if (keyboard_TestF2()==1){
				rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1, 15);
			}

		break;
	case bZap_press:
		if (keyboard_TestF1()==1){
			rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1, 14);
		}
		break;
	case bPS_press:
	if (keyboard_TestF1()==1){
		rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL_MOVE,1, 1);
	}else
	if (keyboard_TestF2()==1){
		rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL_MOVE,1, 2);
	}
	break;
	case bPS_release:
		rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL_MOVE,1, 0);
		break;

  case bNProg_press:
   if (keyboard_TestF2()==1)
   {
    rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,5);
   }
   break;
  case bPlus_press:
   if (keyboard_TestF2()==1)
   {
    ls_ChangeModeDisplay();
   }
   else
   {
    if (ls_ls[ls_currentLs].speed<7) {ls_ls[ls_currentLs].speed++;}
    if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setSpeed,1,ls_ls[ls_currentLs].speed);}
   }
   break;
  case bMinus_press:
   if (ls_ls[ls_currentLs].speed>0) {ls_ls[ls_currentLs].speed--;}
   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setSpeed,1,ls_ls[ls_currentLs].speed);}
   break;
  case bZtOpen_press:
   if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,9);}
   break;
  case bZtClose_press:
   if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,10);}
   break;
  case bZtStop_press:
   if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,FSA,0);}
   break;
  case bNn_press:
   if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,8);}
   else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,nN,0);}}
   break;
  case bNn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);}break;
  case bWn_press:
   if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,7);}
   else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,wN,0);}}
   break;
  case bWn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);}break;
  case bUp_press:
   if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,4);}
   else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,up,0);}}
   break;
  case bUp_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);}break;
  case bDown_press:
   if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,3);}
   else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,down,0);}}
   break;
  case bDown_release: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);}break;
  case bLeft_press:
	if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,2);}
	else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,left,0);}}
   break;
  case bLeft_release: 
	if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);}
	break;
  case bRight_press:
   if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,1);}
   else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,right,0);}}
   break;
  case bRight_release:if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);}break;
 }
 if (keyboard_TestF1()==1)
  {
   if (ls_ls[ls_currentLs].alarm==1)
   {
    f_avar[0]=0;f_avar[1]=0;f_avar[2]=0;Beep_off;lcd_PutString(16,"                ");ls_ls[ls_currentLs].alarm=0;
   }
  }
}

void ls_keyMode_3()
{
	switch (keyboard_GetKey())
	{
		case bZtStop_press: ls_keyMode_3_time = 0;ls_ChangeModeFunc(0);break;
		case bZtOpen_press: ls_keyMode_3_time = 0;rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorOpen,1,0);ls_ChangeModeFunc(0);break;
	}
}

void ls_keyMode_4()
{
	switch (keyboard_GetKey())
	{
		case bZtStop_press: ls_keyMode_4_time = 0;ls_ChangeModeFunc(0);break;
		case bZtOpen_press: ls_keyMode_4_time = 0;rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorOpen,1,1);ls_ChangeModeFunc(0);break;
	}
}

void ls_ProcessInfo(unsigned char nls)
{
 unsigned int coory = (rs485_receiveBuf[9]<<8)+rs485_receiveBuf[10];
 unsigned int coorx = (rs485_receiveBuf[13]<<8)+rs485_receiveBuf[14];
 if ((nls>0)&&(nls<33)){
	if (rs485_receiveBuf[4]==requestInfoFromLS){
   // ps
   if ((rs485_receiveBuf[5]&0x08)!=0) {ls_ls[nls].ps=1;} else {ls_ls[nls].ps=0;}
   // current
   ls_ls[nls].current=rs485_receiveBuf[6];
   // pressure
   ls_ls[nls].pressure=(rs485_receiveBuf[11]<<8)+rs485_receiveBuf[12];
/*   if (ls_ls[nls].pressure<100) {ls_ls[nls].pressure=0;}
   else {ls_ls[nls].pressure-=100;}
   ls_ls[nls].pressure*=10;
   ls_ls[nls].pressure/=78;*/
  }
  else
  {
	  if (rs485_receiveBuf[3] == 3)
		switch (rs485_receiveBuf[5]&0x3c)
		{
			case 0x10: ls_ls[nls].zatvor=1;break;
			case 0x04: ls_ls[nls].zatvor=0;break;
			case 0x00: ls_ls[nls].zatvor=2;break;
			case 0x14: ls_ls[nls].zatvor=3;break;
		}
	  else
	   if (rs485_receiveBuf[7] == 0)
		switch (rs485_receiveBuf[5]&0x3c)
		{
			case 0x10: ls_ls[nls].zatvor=1;break;
			case 0x04: ls_ls[nls].zatvor=0;break;
			case 0x00: ls_ls[nls].zatvor=2;break;
			case 0x14: ls_ls[nls].zatvor=3;break;
		}
	   else
		switch (rs485_receiveBuf[5]&0x3c)
		{
			case 0x10: ls_ls[nls].zatvor1=1;break;
			case 0x04: ls_ls[nls].zatvor1=0;break;
			case 0x00: ls_ls[nls].zatvor1=2;break;
			case 0x14: ls_ls[nls].zatvor1=3;break;
		}
	}
//  if (ls_ProcessInfoCount<ls_ProcessInfoCountMax) {ls_ProcessInfoCount++;}
//  else {ls_ProcessInfoCount=0;ls_PrintDisplay();}
  ls_PrintDisplay();
 }
}

void SetMode()
{
 switch (rs485_receiveBuf[5])
 {
#ifdef EngL         
  case 1: lcd_PutString(16,"install: min gor");break;
  case 2: lcd_PutString(16,"install: max gor");break;
  case 3: lcd_PutString(16,"install:min vert");break;
  case 4: lcd_PutString(16,"install:max vert");break;
  case 5: ls_ChangeModeFunc(0);break;
  case 6: ls_ChangeModeFunc(2);break;
  case 7: lcd_PutString(16,"install: min noz");break;
  case 8: lcd_PutString(16,"install: max noz");break;
  case 9: lcd_PutString(16,"   Zero point    ");break;
  case 10: lcd_PutString(16,"  Limit error   ");break;
  case 11: 
   lcd_PutString(16,"                ");
   lcd_PutNum(18,rs485_receiveBuf[6]);
   lcd_PutNum(23,rs485_receiveBuf[7]);
   break;
#else
  case 1: lcd_PutString(16,"устан: мин гор  ");break;
  case 2: lcd_PutString(16,"устан: макс гор ");break;
  case 3: lcd_PutString(16,"устан: мин верт ");break;
  case 4: lcd_PutString(16,"устан: макс верт");break;
  case 5: ls_ChangeModeFunc(0);break;
  case 6: ls_ChangeModeFunc(2);break;
  case 7: lcd_PutString(16,"устан: мин нас  ");break;
  case 8: lcd_PutString(16,"устан: макс нас ");break;
  case 9: lcd_PutString(16," Нулевая точка  ");break;
  case 10: lcd_PutString(16,"Ошибка пределов ");break;
  case 11: 
   lcd_PutString(16,"                ");
   lcd_PutNum(18,rs485_receiveBuf[6]);
   lcd_PutNum(23,rs485_receiveBuf[7]);
   break;
  case 14: lcd_PutString(16,"Деж. положение  ");break;
  case 15: 
	lcd_PutString(16,   "Пром. положение ");
	break;
  case 16: lcd_PutString(16,"Раб. положение  ");break;
#endif   
 }
}



void ls_TimeFunc()
{
#ifdef BEZ_SK   
  static int nc = 0;
  
  if (nc == 50)
  {
   rs485_receiveBuf[0] = 34;
   rs485_receiveBuf[1] = 33;
   rs485_receiveBuf[2] = 154;
   rs485_receiveBuf[3] = 0;
   rs485_receiveBuf[4] = 221;
   rs485_RecPackageFunc();
   nc = 0;
  }
  else
   nc++;
#endif
  
  if (f2Pr > 0)
  {
	if (f2Pr < 2000)
	  f2Pr++;
	else
	{
	  if (f2Pr == 2000)
	  {
	    f2Pr = 3000;
	  }
	}
  }
  
  if (ls_keyMode_3_time>1) {ls_keyMode_3_time--;}
  else if (ls_keyMode_3_time==1) {ls_keyMode_3_time=0;ls_ChangeModeFunc(0);}

  if (ls_keyMode_4_time>1) {ls_keyMode_4_time--;}
  else if (ls_keyMode_4_time==1) {ls_keyMode_4_time=0;ls_ChangeModeFunc(0);}
}

void elrInfo(unsigned char nls)
{
 if ((nls>0)&&(nls<33))  
 {
  if ((rs485_receiveBuf[4]&0x0f)==0)    {lcd_PutString(32,"                ");}
  if ((rs485_receiveBuf[4]&0x01)==1)    {lcd_PutString(32,"Движение влево  ");}
  if ((rs485_receiveBuf[4]&0x02)==2)    {lcd_PutString(32,"Движение вправо ");}
  if ((rs485_receiveBuf[4]&0x10)==0x10) {lcd_PutString(48,"Движение вверх  ");}
  if ((rs485_receiveBuf[4]&0x20)==0x20) {lcd_PutString(48,"Движение вниз   ");}
  if ((rs485_receiveBuf[4]&0xf0)==0)    {lcd_PutString(48,"                ");}		
  if ((rs485_receiveBuf[4]&0x04)==4)    {lcd_PutString(32,"Левый предел    ");}
  if ((rs485_receiveBuf[4]&0x08)==8)    {lcd_PutString(32,"Правый предел   ");}
  if ((rs485_receiveBuf[4]&0x40)==0x40) {lcd_PutString(48,"Верхний предел  ");}
  if ((rs485_receiveBuf[4]&0x80)==0x80) {lcd_PutString(48,"Нижний предел   ");}
 }
}


//// управление КЛАПАНОМ затвора отключено
//
//
//#include "main.h"
//#include "ls.h"
//#include "keyboard.h"
//#include "lcd.h"
//#include "NetDriver_rs485.h"
//#include "listCommand.h"
//#include "timers.h"
//
//unsigned int f2Pr = 0;
//unsigned int PrNap = 0;
//
//void ls_ChangeModeFunc(unsigned char);
//void ls_ChangeModeDisplay();
//void ls_PrintDisplay();
//
//ls_TLs ls_ls[33];
//unsigned char ls_currentLs;
//
//unsigned char ls_NoSetLS;
//#define ls_SET_NoSetLS ls_NoSetLS++
//#define ls_CLEAR_NoSetLS {if (ls_NoSetLS!=0) {ls_NoSetLS--;}}
//
//void ls_ChangeLs();
//void ls_keyMode_0();
//void ls_keyMode_1();
//void ls_keyMode_2();
//void ls_keyMode_3();
//void ls_keyMode_4();
//int ls_keyMode_3_time;
//int ls_keyMode_4_time;
//void ls_TimeFunc();
//
//unsigned char ls_ProcessInfoCount;
//#define ls_ProcessInfoCountMax 1
//
//void ls_Init()
//{
 //unsigned char i;
 //ls_keyMode_3_time = 0;
 //ls_keyMode_4_time = 0;
//#ifdef BEZ_SK
 //ls_currentLs=1;
//#else
 //ls_currentLs=0;
//#endif 
 //ls_NoSetLS=0;
//#ifdef EngL
 //lcd_PutString(0,"LS N: ");
//#else
 //lcd_PutString(0,"ЛС N: ");
//#endif
 //lcd_PutNum2(5,0);
 //for (i=0;i<33;i++)
 //{
  //ls_ls[i].presence=0;
  //ls_ls[i].current=0;
  //ls_ls[i].pressure=0;
  //ls_ls[i].nProg=0;
  //ls_ls[i].ps=0;
  //ls_ls[i].speed=7;
  //ls_ls[i].ustFS=0;
  //ls_ls[i].alarm=0;
  //ls_ls[i].modeDisplay=0;
  //ls_ls[i].modeFunc=0;
  //ls_ls[i].zatvor=0;
  //ls_ls[i].zatvor1=0;
  //ls_ls[i].step=0;
 //}
 //ls_ProcessInfoCount=0;
 //timers_RegVirtualTimer(&ls_TimeFunc);
//}
//
//void ls_SetLsPresence(unsigned char *list)
//{
 //ls_ls[32].presence=(((*list)&0x80)>>7);
 //ls_ls[31].presence=(((*list)&0x40)>>6);
 //ls_ls[30].presence=(((*list)&0x20)>>5);
 //ls_ls[29].presence=(((*list)&0x10)>>4);
 //ls_ls[28].presence=(((*list)&0x08)>>3);
 //ls_ls[27].presence=(((*list)&0x04)>>2);
 //ls_ls[26].presence=(((*list)&0x02)>>1);
 //ls_ls[25].presence=(((*list)&0x01));
 //ls_ls[24].presence=((*(list+1)&0x80)>>7);
 //ls_ls[23].presence=((*(list+1)&0x40)>>6);
 //ls_ls[22].presence=((*(list+1)&0x20)>>5);
 //ls_ls[21].presence=((*(list+1)&0x10)>>4);
 //ls_ls[20].presence=((*(list+1)&0x08)>>3);
 //ls_ls[19].presence=((*(list+1)&0x04)>>2);
 //ls_ls[18].presence=((*(list+1)&0x02)>>1);
 //ls_ls[17].presence=((*(list+1)&0x01));
 //ls_ls[16].presence=((*(list+2)&0x80)>>7);
 //ls_ls[15].presence=((*(list+2)&0x40)>>6);
 //ls_ls[14].presence=((*(list+2)&0x20)>>5);
 //ls_ls[13].presence=((*(list+2)&0x10)>>4);
 //ls_ls[12].presence=((*(list+2)&0x08)>>3);
 //ls_ls[11].presence=((*(list+2)&0x04)>>2);
 //ls_ls[10].presence=((*(list+2)&0x02)>>1);
 //ls_ls[9].presence=((*(list+2)&0x01));
 //ls_ls[8].presence=((*(list+3)&0x80)>>7);
 //ls_ls[7].presence=((*(list+3)&0x40)>>6);
 //ls_ls[6].presence=((*(list+3)&0x20)>>5);
 //ls_ls[5].presence=((*(list+3)&0x10)>>4);
 //ls_ls[4].presence=((*(list+3)&0x08)>>3);
 //ls_ls[3].presence=((*(list+3)&0x04)>>2);
 //ls_ls[2].presence=((*(list+3)&0x02)>>1);
 //ls_ls[1].presence=((*(list+3)&0x01));
//
 //ls_ls[32].presence|=((*(list+5)&0x80)>>7);
 //ls_ls[31].presence|=((*(list+5)&0x40)>>6);
 //ls_ls[30].presence|=((*(list+5)&0x20)>>5);
 //ls_ls[29].presence|=((*(list+5)&0x10)>>4);
 //ls_ls[28].presence|=((*(list+5)&0x08)>>3);
 //ls_ls[27].presence|=((*(list+5)&0x04)>>2);
 //ls_ls[26].presence|=((*(list+5)&0x02)>>1);
 //ls_ls[25].presence|=((*(list+5)&0x01));
 //ls_ls[24].presence|=((*(list+6)&0x80)>>7);
 //ls_ls[23].presence|=((*(list+6)&0x40)>>6);
 //ls_ls[22].presence|=((*(list+6)&0x20)>>5);
 //ls_ls[21].presence|=((*(list+6)&0x10)>>4);
 //ls_ls[20].presence|=((*(list+6)&0x08)>>3);
 //ls_ls[19].presence|=((*(list+6)&0x04)>>2);
 //ls_ls[18].presence|=((*(list+6)&0x02)>>1);
 //ls_ls[17].presence|=((*(list+6)&0x01));
 //ls_ls[16].presence|=((*(list+7)&0x80)>>7);
 //ls_ls[15].presence|=((*(list+7)&0x40)>>6);
 //ls_ls[14].presence|=((*(list+7)&0x20)>>5);
 //ls_ls[13].presence|=((*(list+7)&0x10)>>4);
 //ls_ls[12].presence|=((*(list+7)&0x08)>>3);
 //ls_ls[11].presence|=((*(list+7)&0x04)>>2);
 //ls_ls[10].presence|=((*(list+7)&0x02)>>1);
 //ls_ls[9].presence|=((*(list+7)&0x01));
 //ls_ls[8].presence|=((*(list+8)&0x80)>>7);
 //ls_ls[7].presence|=((*(list+8)&0x40)>>6);
 //ls_ls[6].presence|=((*(list+8)&0x20)>>5);
 //ls_ls[5].presence|=((*(list+8)&0x10)>>4);
 //ls_ls[4].presence|=((*(list+8)&0x08)>>3);
 //ls_ls[3].presence|=((*(list+8)&0x04)>>2);
 //ls_ls[2].presence|=((*(list+8)&0x02)>>1);
 //ls_ls[1].presence|=((*(list+8)&0x01));
//
 //if ((ls_currentLs!=0)&&(ls_ls[ls_currentLs].presence==0)) {ls_ChangeLs();}
//}
//
//void ls_ChangeLs()
//{
 //do
 //{
  //if (ls_currentLs<32) {ls_currentLs++;}
  //else
  //{
   //ls_currentLs=0;
   //lcd_PutString(0,sEmpty);
//#ifdef EngL
 //lcd_PutString(0,"LS N: ");
//#else
 //lcd_PutString(0,"ЛС N: ");
//#endif
  //}
 //}
 //while ((ls_ls[ls_currentLs].presence==0)&&(ls_currentLs!=0));
 //lcd_PutNum2(5,ls_currentLs);
 //if (ls_currentLs==0)
 //{
  //lcd_PutString(16,"                ");
  //lcd_PutString(32,"                ");
  //lcd_PutString(48,"                ");
 //}
//#ifndef ELR 
 //else 
   //ls_PrintDisplay();
//#endif 
//}
//
//void ls_main()
//{
	  //if (f2Pr == 2000)
	  //{
	    //f2Pr = 3000;
		//if (PrNap == 1)
		//{
			//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL,1,1);ls_SET_NoSetLS;// в рабочее
		//}
		//else
		//{
		   //rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL,1,2);ls_SET_NoSetLS;// в дежурное
		//}
	  //}
	  //else
	  //{
//
 //switch (ls_ls[ls_currentLs].modeFunc)
 //{
  //case 0: ls_keyMode_0();break;
  //case 1: ls_keyMode_1();break;
  //case 2: ls_keyMode_2();break;
  //case 3: ls_keyMode_3();break;
  //case 4: ls_keyMode_4();break;
 //}
//// ls_PrintDisplay();
	  //}
//}
//
//void ls_ChangeModeDisplay()
//{
 //if (ls_ls[ls_currentLs].modeDisplay==1){
	 //ls_ls[ls_currentLs].modeDisplay=7;
 //}else{
	 //if (ls_ls[ls_currentLs].modeDisplay==7){
		 //ls_ls[ls_currentLs].modeDisplay=2;
	 //}else{
			//if (ls_ls[ls_currentLs].modeDisplay<5) {ls_ls[ls_currentLs].modeDisplay++;}
			//else if (ls_ls[ls_currentLs].modeDisplay==5) {ls_ls[ls_currentLs].modeDisplay=0;}
	 //}		 		 	 
 //}
 //ls_PrintDisplay();
//}
//
//void ls_PrintDisplay()
//{
 //if (ls_currentLs!=0)
 //{
  //switch (ls_ls[ls_currentLs].modeDisplay)
  //{
   //case 0: 
//#ifdef EngL     
     //lcd_PutString(32,"Current:      A ");
     //lcd_PutNumf(41,ls_ls[ls_currentLs].current);break;
//#else     
     //lcd_PutString(32,"Ток:      A     ");
     //lcd_PutNumf(37,ls_ls[ls_currentLs].current);break;
//#endif
   //case 1:
//#ifdef EngL
	   //lcd_PutString(32,"Gate:           ");
		//switch (ls_ls[ls_currentLs].zatvor)
		//{
			//case 0: lcd_PutString(41,"Closed  ");break;
			//case 1: lcd_PutString(41,"Opened  ");break;
			//case 2: lcd_PutString(41,"Work    ");break;
			//case 3: lcd_PutString(41,"Crash   ");break;
		//}
		//break;
//#else
	   //lcd_PutString(32,"Затвор 1:       ");
		//switch (ls_ls[ls_currentLs].zatvor)
		//{
			//case 0: lcd_PutString(41,"закрыт  ");break;
			//case 1: lcd_PutString(41,"открыт  ");break;
			//case 2: lcd_PutString(41,"работа  ");break;
			//case 3: lcd_PutString(41,"авария  ");break;
		//}
		//break;
//#endif
   //case 7:
//#ifdef EngL
	   //lcd_PutString(32,"Gate 2:         ");
		//switch (ls_ls[ls_currentLs].zatvor1)
		//{
			//case 0: lcd_PutString(41,"Closed  ");break;
			//case 1: lcd_PutString(41,"Opened  ");break;
			//case 2: lcd_PutString(41,"Work    ");break;
			//case 3: lcd_PutString(41,"Crash   ");break;
		//}
		//break;
//#else
	   //lcd_PutString(32,"Затвор 2:       ");
		//switch (ls_ls[ls_currentLs].zatvor1)
		//{
			//case 0: lcd_PutString(41,"закрыт  ");break;
			//case 1: lcd_PutString(41,"открыт  ");break;
			//case 2: lcd_PutString(41,"работа  ");break;
			//case 3: lcd_PutString(41,"авария  ");break;
		//}
		//break;
//#endif
   //case 2:
//#ifdef EngL     
    //lcd_PutString(32,"Valve:          ");
    //switch (ls_ls[ls_currentLs].valve)
    //{
     //case 0: lcd_PutString(39,"Closed    ");break;
     //case 1: lcd_PutString(39,"Opened    ");break;
    //} 
    //break;
//#else
    //lcd_PutString(32,"Клапан:         ");
    //switch (ls_ls[ls_currentLs].valve)
    //{
     //case 0: lcd_PutString(39,"закрыт    ");break;
     //case 1: lcd_PutString(39,"открыт    ");break;
    //} 
    //break;
//#endif    
//#ifdef EngL     
   //case 3: lcd_PutString(32,"Press:      Atm ");lcd_PutNumf(39,ls_ls[ls_currentLs].pressure);break;
   //case 4: lcd_PutString(32,"Prog num:       ");lcd_PutNum1(42,ls_ls[ls_currentLs].nProg);break;
   //case 5: lcd_PutString(32,"Speed:          ");lcd_PutNum1(42,ls_ls[ls_currentLs].speed+1);break;
   //case 6: lcd_PutString(32,"Step:           ");lcd_PutNum(42,ls_ls[ls_currentLs].step);break;
//#else
   //case 3: lcd_PutString(32,"Дав:        Атм ");lcd_PutNumf(37,ls_ls[ls_currentLs].pressure);break;
   //case 4: lcd_PutString(32,"Ном прог:       ");lcd_PutNum1(42,ls_ls[ls_currentLs].nProg);break;
   //case 5: lcd_PutString(32,"Скорость:       ");lcd_PutNum1(42,ls_ls[ls_currentLs].speed+1);break;
   //case 6: lcd_PutString(32,"Шаг:            ");lcd_PutNum(42,ls_ls[ls_currentLs].step);break;
//#endif   
  //}
  //switch (ls_ls[ls_currentLs].modeFunc)
  //{
   //case 0:
//#ifdef EngL     
    //if (ls_ls[ls_currentLs].ps==1) {lcd_PutString(48,"    Scanning    ");}
//#else
    //if (ls_ls[ls_currentLs].ps==1) {lcd_PutString(48,"  Сканирование  ");}
//#endif    
    //else {ls_ls[ls_currentLs].ps=0;lcd_PutString(48,"                ");}
    //break;
//#ifdef EngL     
   //case 1: lcd_PutString(48,"Program install ");break;
   //case 2: lcd_PutString(48,"   Settings     ");break;
   //case 3: lcd_PutString(48,"  Open a gate?  ");break;
   //case 4: lcd_PutString(48,"  Open a gate?  ");break;
//#else
   //case 1: lcd_PutString(48,"Устан. программы");break;
   //case 2: lcd_PutString(48,"   Настройки    ");break;
   //case 3: lcd_PutString(48,"Открыть затвор1?");break;
   //case 4: lcd_PutString(48,"Открыть затвор2?");break;
//#endif    
  //}
 //}
//}
//
//void ls_ChangeModeFunc(unsigned char nmode)
//{
 //switch (nmode)
 //{
  //case 0: ls_CLEAR_NoSetLS;ls_ls[ls_currentLs].modeDisplay=0;lcd_PutString(16,"                ");break;
  //case 1: ls_SET_NoSetLS;ls_ls[ls_currentLs].modeDisplay=6;break;
  //case 2: ls_SET_NoSetLS;break;
  //case 3: ls_SET_NoSetLS;ls_keyMode_3_time = 10000;break;
  //case 4: ls_SET_NoSetLS;ls_keyMode_4_time = 10000;break;
 //}
 //ls_ls[ls_currentLs].modeFunc=nmode;
//}
//
//void ls_keyMode_0()
//{
 //switch (keyboard_GetKey())
 //{
//#ifdef FIRE_KEY   
  //case bVid_press:
   //rs485_FillPackageG(ls_currentLs,rs485_lsAddress,FIRE,10,0,45,1,0x3b,1,0x3b,0,45,0,2);
   //break;
//#endif
//#ifndef ELR
  //case bZap_press:
   //if (ls_ls[ls_currentLs].ps==0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setProgPoint1,0);ls_ChangeModeFunc(1);}
   //break;
  //case bNProg_press:
   //if (keyboard_TestF2()==1)
   //{
    //rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,6);
   //}
   //else
   //{
    //if (ls_ls[ls_currentLs].nProg<7) {ls_ls[ls_currentLs].nProg++;} else {ls_ls[ls_currentLs].nProg=0;}
    //ls_PrintDisplay();
   //}
   //break;
  //case bPlus_press:
   //if (keyboard_TestF2()==1)
   //{
    //ls_ChangeModeDisplay();
   //}
   //else
   //{
    //if (ls_ls[ls_currentLs].speed<7) {ls_ls[ls_currentLs].speed++;}
    //if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setSpeed,1,ls_ls[ls_currentLs].speed);}
   //}
   //break;
  //case bMinus_press:
   //if (ls_ls[ls_currentLs].speed>0) {ls_ls[ls_currentLs].speed--;}
   //if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setSpeed,1,ls_ls[ls_currentLs].speed);}
   //break;
  //case bZtOpen_press: 
	//if (ls_currentLs!=0){
		//if (keyboard_TestF1()==1){
			////if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,KLAPAN,1,1);}
			//ls_ChangeModeFunc(4);
		//}else{ 
			//if (keyboard_TestF2()==1){
				//ls_ChangeModeFunc(3);
			//}
		//}			
   //}		
   //break; 
  //case bZtClose_press: 
   //if (keyboard_TestF1()==1)
   //{
    ////if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,KLAPAN,1,0);}
		//if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorClose,1,1);}	   
	//}  
   //else 
   //if (keyboard_TestF2()==1)
   //{
     //if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorClose,0);}
   //}
   //break;
  //case bZtStop_press:  
	//if (ls_currentLs!=0){
		//if (keyboard_TestF1()==1){
			//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorStop,1,1);
		//}else{
			//if (keyboard_TestF2()==1){
				//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorStop,0);
			//}				
		//}			
	//}
	//break;
  //case bNn_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,nN,0);ls_SET_NoSetLS;}break;
  //case bNn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);ls_CLEAR_NoSetLS;}break;
  //case bWn_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,wN,0);ls_SET_NoSetLS;}break;
  //case bWn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);ls_CLEAR_NoSetLS;}break;
//#endif
 //case bUp_press:     
	  //if (ls_currentLs!=0) 
	  //{
		  //if (keyboard_TestF1()==1)
		  //{
			  //f2Pr = 1;
			  //PrNap = 1;
		  //}
		  //else
		  //{
			  //rs485_FillPackageG(ls_currentLs,rs485_lsAddress,up,0);ls_SET_NoSetLS;
		  //}
	  //}
	  //break;
  //case bUp_release:   
	  //if (ls_currentLs!=0) 
	  //{
		  //if (PrNap == 0)
		  //{
			//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);
			//ls_CLEAR_NoSetLS;
		  //}
		  //else
		  //{
			  //PrNap = 0;
			  //f2Pr = 0;
			//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL,1, 0);ls_CLEAR_NoSetLS; // стоп поворотный
		  //}
	  //}
	  //break;
  //case bDown_press:   
	  //if (ls_currentLs!=0) 
	  //{
		  //if (keyboard_TestF1()==1)
		  //{
			  //f2Pr = 1;
			  //PrNap = 2;
		  //}
		  //else
		  //{
			  //rs485_FillPackageG(ls_currentLs,rs485_lsAddress,down,0);ls_SET_NoSetLS;
		  //}
	  //}
	  //break;
  //case bDown_release: 
	  //if (ls_currentLs!=0) 
	  //{
		  //if (PrNap == 0)
		  //{
			//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);
			//ls_CLEAR_NoSetLS;
		  //}
		  //else
		  //{
			  //PrNap = 0;
			  //f2Pr = 0;
			//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL,1, 0);ls_CLEAR_NoSetLS; // стоп поворотный
		  //}
	  //}
	  //break;
  //case bLeft_press:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,left,0);ls_SET_NoSetLS;}break;
  //case bLeft_release: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);ls_CLEAR_NoSetLS;}break;
  //case bRight_press:  if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,right,0);ls_SET_NoSetLS;}break;
  //case bRight_release:if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);ls_CLEAR_NoSetLS;}break;
//#ifndef BEZ_SK     
  //case bLsN_press:    if (ls_NoSetLS==0) {ls_ChangeLs();}break;
//#endif
//#ifndef ELR
 //case bPS_press:
   //if (ls_currentLs!=0)
   //{
     //if (ls_ls[ls_currentLs].ps==0) 
     //{
      //rs485_FillPackageG(ls_currentLs,rs485_lsAddress,startProg,1,ls_ls[ls_currentLs].nProg);
     //}
     //else {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopProg,0);}
   //}
   //break;
//#endif   
 //}
 //if (keyboard_TestF1()==1)
  //{
   //if (ls_ls[ls_currentLs].alarm==1)
   //{
    //f_avar[0]=0;f_avar[1]=0;f_avar[2]=0;Beep_off;lcd_PutString(16,"                ");ls_ls[ls_currentLs].alarm=0;
   //}
  //}
//}
//
//void ls_keyMode_1()
//{
 //switch (keyboard_GetKey())
 //{
  //case bZap_press:
   //rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setProgPoint2,2,ls_ls[ls_currentLs].step,ls_ls[ls_currentLs].nProg);
   //ls_ChangeModeFunc(0);
   //break;
//
  //case bPlus_press:  
   //if (ls_ls[ls_currentLs].step<90) {ls_ls[ls_currentLs].step++;}
   //ls_PrintDisplay();
   //break;
  //case bMinus_press: 
   //if (ls_ls[ls_currentLs].step>0) {ls_ls[ls_currentLs].step--;}
   //ls_PrintDisplay();
   //break;
  //case bZtOpen_press: 
   //if (keyboard_TestF1()==1)
   //{
    ////if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,KLAPAN,1,1);}
   //}  
   //else 
   //if (keyboard_TestF1()==1)
   //{
     ////ls_ChangeModeFunc(3);
   //}
    //break; 
  //case bZtClose_press: 
   //if (keyboard_TestF1()==1)
   //{
    ////if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,KLAPAN,1,0);}
   //}  
   //else 
   //if (keyboard_TestF1()==1)
   //{
     ////if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorClose,0);}
   //}
   //break;
  ////case bZtStop_press: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorStop,0);}break;
  //case bNn_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,nN,0);}break;
  //case bNn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);}break;
  //case bWn_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,wN,0);}break;
  //case bWn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);}break;
  //case bUp_press:     if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,up,0);}break;
  //case bUp_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);}break;
  //case bDown_press:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,down,0);}break;
  //case bDown_release: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);}break;
  //case bLeft_press:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,left,0);}break;
  //case bLeft_release: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);}break;
  //case bRight_press:  if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,right,0);}break;
  //case bRight_release:if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);}break;
 //}
 //if (keyboard_TestF1()==1)
  //{
   //if (ls_ls[ls_currentLs].alarm==1)
   //{
    //f_avar[0]=0;f_avar[1]=0;f_avar[2]=0;Beep_off;lcd_PutString(16,"                ");ls_ls[ls_currentLs].alarm=0;
   //}
  //}
//}
//void ls_keyMode_2()
//{
 //switch (keyboard_GetKey())
 //{
	//case bVid_press:
		//if (keyboard_TestF1()==1){
			//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1, 16);
		//}else
			//if (keyboard_TestF2()==1){
				//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1, 15);
			//}
//
		//break;
	//case bZap_press:
		//if (keyboard_TestF1()==1){
			//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1, 14);
		//}
		//break;
	//case bPS_press:
	//if (keyboard_TestF1()==1){
		//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL_MOVE,1, 1);
	//}else
	//if (keyboard_TestF2()==1){
		//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL_MOVE,1, 2);
	//}
	//break;
	//case bPS_release:
		//rs485_FillPackageG(ls_currentLs,rs485_lsAddress,POVOROTNIY_UZEL_MOVE,1, 0);
		//break;
//
  //case bNProg_press:
   //if (keyboard_TestF2()==1)
   //{
    //rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,5);
   //}
   //break;
  //case bPlus_press:
   //if (keyboard_TestF2()==1)
   //{
    //ls_ChangeModeDisplay();
   //}
   //else
   //{
    //if (ls_ls[ls_currentLs].speed<7) {ls_ls[ls_currentLs].speed++;}
    //if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setSpeed,1,ls_ls[ls_currentLs].speed);}
   //}
   //break;
  //case bMinus_press:
   //if (ls_ls[ls_currentLs].speed>0) {ls_ls[ls_currentLs].speed--;}
   //if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,setSpeed,1,ls_ls[ls_currentLs].speed);}
   //break;
  //case bZtOpen_press:
   //if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,9);}
   //break;
  //case bZtClose_press:
   //if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,10);}
   //break;
  //case bZtStop_press:
   //if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,FSA,0);}
   //break;
  //case bNn_press:
   //if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,8);}
   //else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,nN,0);}}
   //break;
  //case bNn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);}break;
  //case bWn_press:
   //if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,7);}
   //else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,wN,0);}}
   //break;
  //case bWn_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopN,0);}break;
  //case bUp_press:
   //if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,4);}
   //else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,up,0);}}
   //break;
  //case bUp_release:   if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);}break;
  //case bDown_press:
   //if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,3);}
   //else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,down,0);}}
   //break;
  //case bDown_release: if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopV,0);}break;
  //case bLeft_press:
	//if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,2);}
	//else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,left,0);}}
   //break;
  //case bLeft_release: 
	//if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);}
	//break;
  //case bRight_press:
   //if (keyboard_TestF2()==1) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,reqSetMode,1,1);}
   //else {if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,right,0);}}
   //break;
  //case bRight_release:if (ls_currentLs!=0) {rs485_FillPackageG(ls_currentLs,rs485_lsAddress,stopG,0);}break;
 //}
 //if (keyboard_TestF1()==1)
  //{
   //if (ls_ls[ls_currentLs].alarm==1)
   //{
    //f_avar[0]=0;f_avar[1]=0;f_avar[2]=0;Beep_off;lcd_PutString(16,"                ");ls_ls[ls_currentLs].alarm=0;
   //}
  //}
//}
//
//void ls_keyMode_3()
//{
	//switch (keyboard_GetKey())
	//{
		//case bZtStop_press: ls_keyMode_3_time = 0;ls_ChangeModeFunc(0);break;
		//case bZtOpen_press: ls_keyMode_3_time = 0;rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorOpen,0);ls_ChangeModeFunc(0);break;
	//}
//}
//
//void ls_keyMode_4()
//{
	//switch (keyboard_GetKey())
	//{
		//case bZtStop_press: ls_keyMode_4_time = 0;ls_ChangeModeFunc(0);break;
		//case bZtOpen_press: ls_keyMode_4_time = 0;rs485_FillPackageG(ls_currentLs,rs485_lsAddress,zatvorOpen,1,1);ls_ChangeModeFunc(0);break;
	//}
//}
//
//void ls_ProcessInfo(unsigned char nls)
//{
 //unsigned int coory = (rs485_receiveBuf[9]<<8)+rs485_receiveBuf[10];
 //unsigned int coorx = (rs485_receiveBuf[13]<<8)+rs485_receiveBuf[14];
 //if ((nls>0)&&(nls<33)){
	//if (rs485_receiveBuf[4]==requestInfoFromLS){
   //// ps
   //if ((rs485_receiveBuf[5]&0x08)!=0) {ls_ls[nls].ps=1;} else {ls_ls[nls].ps=0;}
   //// current
   //ls_ls[nls].current=rs485_receiveBuf[6];
   //// pressure
   //ls_ls[nls].pressure=(rs485_receiveBuf[11]<<8)+rs485_receiveBuf[12];
///*   if (ls_ls[nls].pressure<100) {ls_ls[nls].pressure=0;}
   //else {ls_ls[nls].pressure-=100;}
   //ls_ls[nls].pressure*=10;
   //ls_ls[nls].pressure/=78;*/
  //}
  //else
  //{
		//switch (rs485_receiveBuf[5]&0x3c)
		//{
			//case 0x10: ls_ls[nls].zatvor=1;break;
			//case 0x04: ls_ls[nls].zatvor=0;break;
			//case 0x00: ls_ls[nls].zatvor=2;break;
			//case 0x14: ls_ls[nls].zatvor=3;break;
		//}
		//switch (rs485_receiveBuf[7]&0x3c)
		//{
			//case 0x10: ls_ls[nls].zatvor1=1;break;
			//case 0x04: ls_ls[nls].zatvor1=0;break;
			//case 0x00: ls_ls[nls].zatvor1=2;break;
			//case 0x14: ls_ls[nls].zatvor1=3;break;
		//}
	//}
////  if (ls_ProcessInfoCount<ls_ProcessInfoCountMax) {ls_ProcessInfoCount++;}
////  else {ls_ProcessInfoCount=0;ls_PrintDisplay();}
  //ls_PrintDisplay();
 //}
//}
//
//void SetMode()
//{
 //switch (rs485_receiveBuf[5])
 //{
//#ifdef EngL         
  //case 1: lcd_PutString(16,"install: min gor");break;
  //case 2: lcd_PutString(16,"install: max gor");break;
  //case 3: lcd_PutString(16,"install:min vert");break;
  //case 4: lcd_PutString(16,"install:max vert");break;
  //case 5: ls_ChangeModeFunc(0);break;
  //case 6: ls_ChangeModeFunc(2);break;
  //case 7: lcd_PutString(16,"install: min noz");break;
  //case 8: lcd_PutString(16,"install: max noz");break;
  //case 9: lcd_PutString(16,"   Zero point    ");break;
  //case 10: lcd_PutString(16,"  Limit error   ");break;
  //case 11: 
   //lcd_PutString(16,"                ");
   //lcd_PutNum(18,rs485_receiveBuf[6]);
   //lcd_PutNum(23,rs485_receiveBuf[7]);
   //break;
//#else
  //case 1: lcd_PutString(16,"устан: мин гор  ");break;
  //case 2: lcd_PutString(16,"устан: макс гор ");break;
  //case 3: lcd_PutString(16,"устан: мин верт ");break;
  //case 4: lcd_PutString(16,"устан: макс верт");break;
  //case 5: ls_ChangeModeFunc(0);break;
  //case 6: ls_ChangeModeFunc(2);break;
  //case 7: lcd_PutString(16,"устан: мин нас  ");break;
  //case 8: lcd_PutString(16,"устан: макс нас ");break;
  //case 9: lcd_PutString(16," Нулевая точка  ");break;
  //case 10: lcd_PutString(16,"Ошибка пределов ");break;
  //case 11: 
   //lcd_PutString(16,"                ");
   //lcd_PutNum(18,rs485_receiveBuf[6]);
   //lcd_PutNum(23,rs485_receiveBuf[7]);
   //break;
  //case 14: lcd_PutString(16,"Деж. положение  ");break;
  //case 15: 
	//lcd_PutString(16,   "Пром. положение ");
	//break;
  //case 16: lcd_PutString(16,"Раб. положение  ");break;
//#endif   
 //}
//}
//
//
//
//void ls_TimeFunc()
//{
//#ifdef BEZ_SK   
  //static int nc = 0;
  //
  //if (nc == 50)
  //{
   //rs485_receiveBuf[0] = 34;
   //rs485_receiveBuf[1] = 33;
   //rs485_receiveBuf[2] = 154;
   //rs485_receiveBuf[3] = 0;
   //rs485_receiveBuf[4] = 221;
   //rs485_RecPackageFunc();
   //nc = 0;
  //}
  //else
   //nc++;
//#endif
  //
  //if (f2Pr > 0)
  //{
	//if (f2Pr < 2000)
	  //f2Pr++;
	//else
	//{
	  //if (f2Pr == 2000)
	  //{
	    //f2Pr = 3000;
	  //}
	//}
  //}
  //
  //if (ls_keyMode_3_time>1) {ls_keyMode_3_time--;}
  //else if (ls_keyMode_3_time==1) {ls_keyMode_3_time=0;ls_ChangeModeFunc(0);}
//
  //if (ls_keyMode_4_time>1) {ls_keyMode_4_time--;}
  //else if (ls_keyMode_4_time==1) {ls_keyMode_4_time=0;ls_ChangeModeFunc(0);}
//}
//
//void elrInfo(unsigned char nls)
//{
 //if ((nls>0)&&(nls<33))  
 //{
  //if ((rs485_receiveBuf[4]&0x0f)==0)    {lcd_PutString(32,"                ");}
  //if ((rs485_receiveBuf[4]&0x01)==1)    {lcd_PutString(32,"Движение влево  ");}
  //if ((rs485_receiveBuf[4]&0x02)==2)    {lcd_PutString(32,"Движение вправо ");}
  //if ((rs485_receiveBuf[4]&0x10)==0x10) {lcd_PutString(48,"Движение вверх  ");}
  //if ((rs485_receiveBuf[4]&0x20)==0x20) {lcd_PutString(48,"Движение вниз   ");}
  //if ((rs485_receiveBuf[4]&0xf0)==0)    {lcd_PutString(48,"                ");}		
  //if ((rs485_receiveBuf[4]&0x04)==4)    {lcd_PutString(32,"Левый предел    ");}
  //if ((rs485_receiveBuf[4]&0x08)==8)    {lcd_PutString(32,"Правый предел   ");}
  //if ((rs485_receiveBuf[4]&0x40)==0x40) {lcd_PutString(48,"Верхний предел  ");}
  //if ((rs485_receiveBuf[4]&0x80)==0x80) {lcd_PutString(48,"Нижний предел   ");}
 //}
//}
//