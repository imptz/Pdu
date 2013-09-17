void ls_Init();

typedef struct
{
  unsigned char presence;
  unsigned int current;
  unsigned int pressure;
  unsigned char nProg;
  unsigned char ps;
  unsigned char speed;
  unsigned int ustFS;
  unsigned char alarm;
  unsigned char modeDisplay;
  unsigned char modeFunc;
  unsigned char zatvor;
  unsigned char zatvor1;
  unsigned char step;
  unsigned char valve;
} ls_TLs;

extern ls_TLs ls_ls[33];

void ls_SetLsPresence(unsigned char *);
void ls_main();
extern unsigned char ls_currentLs;
void ls_ProcessInfo(unsigned char);
void SetMode();
void elrInfo(unsigned char);

