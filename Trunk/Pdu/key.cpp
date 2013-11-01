#include "key.h"

Key::PHASE Key::phase;
int Key::keysCnt[Key::COL_NUMBER][Key::ROW_NUMBER];
Key::KEY_STATE Key::keysState[Key::COL_NUMBER][Key::ROW_NUMBER];
unsigned int Key::activeRow;
Fifo Key::keyFifo;

bool Key::f1State = false;
bool Key::f2State = false;

const unsigned char Key::KEY_CODE_MATRIX_PRESS[Key::COL_NUMBER][Key::ROW_NUMBER] = {
	{0, 0, bZtOpen_press, bPlus_press, bWn_press, bRight_press},
	{bPS_press, bVid_press, bZtStop_press, bMinus_press, bDown_press, bUp_press},
	{bNProg_press, bZap_press, bZtClose_press, bLsN_press, bNn_press, bLeft_press}
};

const unsigned char Key::KEY_CODE_MATRIX_RELEASE[Key::COL_NUMBER][Key::ROW_NUMBER] = {
	{0, 0, bZtOpen_release, bPlus_release, bWn_release, bRight_release},
	{bPS_release, bVid_release, bZtStop_release, bMinus_release, bDown_release, bUp_release},
	{bNProg_release, bZap_release, bZtClose_release, bLsN_release, bNn_release, bLeft_release}
};

void keyTimerFunc(void){
	Key::timer();
}

void Key::init(){
	phase = PHASE_FREE;
	activeRow = 0;
	
	for (unsigned int c = 0; c < COL_NUMBER; ++c)
		for (unsigned int r = 0; r < ROW_NUMBER; ++r){
			keysCnt[c][r] = 0;	
			keysState[c][r] = KEY_STATE_RELEASE;
		}			
	
	off();
	timers_RegVirtualTimer(&keyTimerFunc);
}

void Key::off(){
	DDRB &= 0xc0;
	PORTB |= 0x3f;
	DDRE &= 0x1f;
	PORTE |= 0xe0;
}

void Key::rowOn(unsigned int rowNumber){
	off();
	DDRB |= (1 << rowNumber);
	PORTB &= (~(1 << rowNumber) & 0x3f | 0xc0);
}

unsigned char Key::getRow(){
	return (PINE & 0xe0) >> 5;
}

void Key::setKeysCnt(unsigned char row, unsigned char rowNumber){
	if (!(row & (1 << rowNumber))){
		if (keysCnt[rowNumber][activeRow] < MAX_PIN_CNT)
		keysCnt[rowNumber][activeRow]++;
	}else{
		if (keysCnt[rowNumber][activeRow] > 0)
		keysCnt[rowNumber][activeRow]--;
	}	
}

void Key::addKeyAction(unsigned char col, unsigned char row, KEY_STATE action){
	if ((col == 0) && (row == 0)){
		if (action == KEY_STATE_PRESS)
			f2State = true;
		else
			f2State = false;
	}else if ((col == 0) && (row == 1)){
		if (action == KEY_STATE_PRESS)
			f1State = true;
		else
			f1State = false;
	}else if (action == KEY_STATE_PRESS)
			keyFifo.put(KEY_CODE_MATRIX_PRESS[col][row]);
			else
				keyFifo.put(KEY_CODE_MATRIX_RELEASE[col][row]);

	keysState[col][row] = action;
}

void Key::keysDetect(){
	for (unsigned int c = 0; c < COL_NUMBER; ++c)
		for (unsigned int r = 0; r < ROW_NUMBER; ++r){
			if ((keysCnt[c][r] <= RELEASE_LIMIT) && (keysState[c][r] == KEY_STATE_PRESS))
				addKeyAction(c, r, KEY_STATE_RELEASE);
			else if ((keysCnt[c][r] >= PRESS_LIMIT) && (keysState[c][r] == KEY_STATE_RELEASE))
					addKeyAction(c, r, KEY_STATE_PRESS);
		}					
}

void Key::timer(){
	static unsigned char ccc = 0;
	
	unsigned char row;
	
	switch(phase){
		case PHASE_FREE:
			phase = PHASE_SET_OUT_PAUSE;
			off();
			break;
		case PHASE_SET_OUT_PAUSE:
			phase = PHASE_GET_IN;
			rowOn(activeRow);
			break;
		case PHASE_GET_IN:
			phase = PHASE_GET_IN;
			row = getRow();
			
			setKeysCnt(row, 0);
			setKeysCnt(row, 1);
			setKeysCnt(row, 2);
			
			keysDetect();
			
			if (++activeRow == ROW_NUMBER)
				activeRow = 0;
				
			rowOn(activeRow);
			break;
	}
}

char keyboard_GetKey(void){
	unsigned char k;
	if (Key::keyFifo.get(&k) > 0)
		return k;
		
	return 0;
}

//char keyboard_TestKey(void){
	//
//}
//
//void keyboard_ClearKeyQueue(){
	//
//}
//
//unsigned char keyboard_GetFlags(){
	//
//}
//
unsigned char keyboard_TestF1(){
	if (Key::f1State)
		return 1;
	
	return 0;
}

unsigned char keyboard_TestF2(){
	if (Key::f2State)
		return 1;
	
	return 0;
}

