
typedef void (*TPointToHandlerADCInterrupt)(void);
extern void RegChannel(TPointToHandlerADCInterrupt fpoint,unsigned char channel);
extern void InitADC(void);
