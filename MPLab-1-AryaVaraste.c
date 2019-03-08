
/*  
   LCD16x2 4 bit ATmega16 interface
   http://www.electronicwings.com
*/



#define F_CPU 8000000UL					/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>						/* Include AVR std. library file */
#include <util/delay.h>					/* Include inbuilt defined Delay header file */

#define LCD_Dir DDRC					/* Define LCD data port direction */
#define LCD_Port PORTC					/* Define LCD data port */
#define RS PC1							/* Define Register Select (data reg./command reg.) signal pin */
#define EN PC3 							/* Define Enable signal pin */
 

void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0); /* sending upper nibble */
	LCD_Port &= ~ (1<<RS);				/* RS=0, command reg. */
	LCD_Port |= (1<<EN);				/* Enable pulse */
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);

	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);  /* sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}


void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
	LCD_Port |= (1<<RS);				/* RS=1, data reg. */
	LCD_Port|= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);

	_delay_us(200);

	LCD_Port = (LCD_Port & 0x0F) | (data << 4); /* sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Init (void)					/* LCD Initialize function */
{
	LCD_Dir = 0xFF;						/* Make LCD command port direction as o/p */
	_delay_ms(20);						/* LCD Power ON delay always >15ms */
	
	LCD_Command(0x33);
	LCD_Command(0x32);		    		/* send for 4 bit initialization of LCD  */
	LCD_Command(0x28);              	/* Use 2 line and initialize 5*7 matrix in (4-bit mode)*/
	LCD_Command(0x0c);              	/* Display on cursor off*/
	LCD_Command(0x06);              	/* Increment cursor (shift cursor to right)*/
	LCD_Command(0x01);              	/* Clear display screen*/
	_delay_ms(2);
	LCD_Command (0x80);					/* Cursor 1st row 0th position */
}


void LCD_String (char *str)				/* Send string to LCD function */
{
	int i;
	for(i=0;str[i]!=0;i++)				/* Send each char of string till the NULL */
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	/* Send string to LCD with xy position */
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);		/* Command of first row and required position<16 */
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);		/* Command of first row and required position<16 */
	LCD_String(str);					/* Call LCD string function */
}

void LCD_Clear()
{
	LCD_Command (0x01);					/* Clear display */
	_delay_ms(2);
	LCD_Command (0x80);					/* Cursor 1st row 0th position */
}
 
int main()
{
	LCD_Init();
    DDRA = 0;
    ADMUX |=(1<<REFS0)|(1<<REFS1);
    ADCSRA |=(1<<ADEN)|(1<<ADATE)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);
    
    int16_t COUNTA = 0;
    char SHOWA [3];

    ADCSRA |=(1<<ADSC);

    while(1)
    {
        COUNTA = ADC/4;
        LCD_String ("CIRCUIT DIGEST");
        LCD_Command(0x80 + 0x40 + 0);
        LCD_String ("Temp(C)=");
        LCD_Command(0x80 + 0x40 + 8);
        itoa(COUNTA,SHOWA,10);
        LCD_String(SHOWA);
        LCD_String ("      ");
        LCD_Command(0x80 + 0);
        
    }    
	return 0;

}
 
