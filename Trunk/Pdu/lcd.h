void lcd_InitLCD(void);
void lcd_PutChar(char ,char );
void lcd_ClearLCD(void);
void lcd_PutString(char , char*);
void lcd_PutNum(unsigned char ,unsigned char );
void lcd_PutNum2(unsigned char,unsigned char);
void lcd_PutNum1(unsigned char ,unsigned char);
void lcd_PutNumf(unsigned char ,unsigned char);
void lcd_PutNumf2(unsigned char,unsigned int);

#define lcd_LEDOn  DDRG|=0x1f;PORTG|=0x1f
#define lcd_LEDOff DDRG|=0x1f;PORTG&=0xe0
#define lcd_VDDOn  DDRD|=0x0c;PORTD|=0x0c
#define lcd_VDDOff DDRD|=0x0c;PORTD&=0xf3
#define lcd_VEEOn  DDRD|=0x10;PORTD&=0xef
#define lcd_VEEOff DDRD|=0x10;PORTD|=0x10


