#ifndef KEY_H_
#define KEY_H_

#include "main.h"
#include "timers.h"
#include "lcd.h"
#include "fifo.h"

#define bNn_press 0x0c
#define bNn_release 0x8c
#define bWn_press 0x20
#define bWn_release 0xa0

#define bUp_press 0x15
#define bUp_release 0x95
#define bDown_press 0x16
#define bDown_release 0x96

#define bLeft_press 0x0b
#define bLeft_release 0x8b
#define bRight_press 0x1f
#define bRight_release 0x9f

#define bZtOpen_press 0x22
#define bZtClose_press 0x0e
#define bZtStop_press 0x18
#define bZtOpen_release 0xa2
#define bZtClose_release 0x8e
#define bZtStop_release 0x98

#define bLsN_press 0x0d
#define bLsN_release 0x8d

#define bPS_press 0x1a
#define bPS_release 0x9a

#define bPlus_press 0x21
#define bPlus_release 0xa1
#define bMinus_press 0x17
#define bMinus_release 0x97

#define bZap_press 0x0f
#define bNProg_press 0x10
#define bVid_press 0x19
#define bZap_release 0x8f
#define bNProg_release 0x90
#define bVid_release 0x99

class Key{
public:
	enum PHASE{
		PHASE_FREE,
		PHASE_SET_OUT_PAUSE,
		PHASE_GET_IN
	};
	static PHASE phase;
	
	static Fifo keyFifo;

	static void off();
	static void rowOn(unsigned int rowNumber);
	static unsigned char getRow();

	static const unsigned int COL_NUMBER = 3;
	static const unsigned int ROW_NUMBER = 6;
	static int keysCnt[COL_NUMBER][ROW_NUMBER];

	enum KEY_STATE{
		KEY_STATE_PRESS = 1,
		KEY_STATE_RELEASE = 2
	};

	static const unsigned int MAX_PIN_CNT = 20;
	static KEY_STATE keysState[COL_NUMBER][ROW_NUMBER];
	static const unsigned int PRESS_LIMIT = MAX_PIN_CNT * 8 / 10;
	static const unsigned int RELEASE_LIMIT = MAX_PIN_CNT * 2 / 10;

	static const unsigned char KEY_CODE_MATRIX_PRESS[COL_NUMBER][ROW_NUMBER];
	static const unsigned char KEY_CODE_MATRIX_RELEASE[COL_NUMBER][ROW_NUMBER];

	static unsigned int activeRow;
	static void setKeysCnt(unsigned char row, unsigned char rowNumber);
	static void keysDetect();
	static void addKeyAction(unsigned char col, unsigned char row, KEY_STATE action);

public:
	static bool f1State;
	static bool f2State;

	static void init();
	static void timer();
};

void keyTimerFunc(void);

char keyboard_GetKey(void);
//char keyboard_TestKey(void);
//void keyboard_ClearKeyQueue();
//unsigned char keyboard_GetFlags();
unsigned char keyboard_TestF1();
unsigned char keyboard_TestF2();

#endif