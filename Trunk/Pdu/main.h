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

#define sNasMove     "�������� �������"
#define sNasAv       "������ ��� ���  "
#define sNasAvBreak  "������ ����� ���"

#define sVerMove     "�������� �� ����"
#define sUpLimit     "������� ������  "
#define sDownLimit   "������ ������   "
#define sVerAvarSens "������ ��� ���� "
#define sVerAv       "������ ��� ���� "
#define sVerAvBreak  "������ ����� ���"

#define sGorMove     "�������� �� ��� "
#define sLeftLimit   "����� ������    "
#define sRightLimit  "������ ������   "
#define sGorAvarSens "������ ��� ���  "
#define sGorAv       "������ ��� ���  "
#define sGorAvBreak  "������ ����� ���"

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