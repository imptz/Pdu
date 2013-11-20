#ifndef MAIN_H
#define MAIN_H

#include <avr/io.h> 
#include <avr/interrupt.h>

extern int qqq;

#define sEmpty  "                "
#define sEmpty8 "        "
#define sA "A"
#define sV "V"
#define sC "S"

#define sNasMove     "Движение насадка"
#define sNasAv       "Авария ток нас  "
#define sNasAvBreak  "Авария обрыв нас"

#define sVerMove     "Движение по верт"
#define sUpLimit     "Верхний предел  "
#define sDownLimit   "Нижний предел   "
#define sVerAvarSens "Авария дат верт "
#define sVerAv       "Авария ток верт "
#define sVerAvBreak  "Авария обрыв вер"

#define sGorMove     "Движение по гор "
#define sLeftLimit   "Левый предел    "
#define sRightLimit  "Правый предел   "
#define sGorAvarSens "Авария дат гор  "
#define sGorAv       "Авария ток гор  "
#define sGorAvBreak  "Авария обрыв гор"

extern unsigned char f_avar[3];

#define Beep_on DDRD|=0x03;PORTD|=0x03
#define Beep_off DDRD|=0x03;PORTD&=0xfc

//#define EngL
//#define COORDVIEW
#define POVOROT
//#define BEZ_SK
//#define ELR
//#define FIRE_KEY
#define ZATVOR_2

#endif