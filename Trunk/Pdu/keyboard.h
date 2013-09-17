int keyboard_Init();
char keyboard_GetKey(void);
char keyboard_TestKey(void);
void keyboard_ClearKeyQueue();
unsigned char keyboard_GetFlags();
unsigned char keyboard_TestF1();
unsigned char keyboard_TestF2();

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


extern char keyboard_oldKeyStatea;
extern char keyboard_oldKeyStateb;
extern char keyboard_oldKeyStatec;
