//===============================================================================
//
//	Author				: Cytron Technologies
//	Project				: LCD Tutorial, Robot.Head2Toe magazine.
//	Project description	: Creating custom characters for 16x2 character LCD 
//						  type HD44580.						  
//
//	Microcontroller		: PIC16F877A
//	Compiler			: HI-TECH C PRO for the PIC101216 MCU Family
//	Osc frequency		: 20MHz
//
//===============================================================================

#include <pic.h>

__CONFIG ( 0x3F32 );		//uC configuration bits

#define LCD_data 	PORTD	//LCD 8-bit data PORT
#define LCD_DB7   	RD7		//busy flag in 
#define LCD_rs   	RB5
#define LCD_rw   	RB6
#define LCD_en   	RB7


//===============================================================================
//	Function Prototypes
//===============================================================================
void LCD_init();
void LCD_busy();
void LCD_command(unsigned char var);
void LCD_senddata(unsigned char var);
void LCD_sendstring(const char *var);


//===============================================================================
//	Main Program
//===============================================================================
void main()
{
	unsigned char i;

	TRISB = 0x1F;				//PORTB bit 5 to 7 as o/p, else i/p
	TRISD = 0x00;				//PORTD as o/p
	
	LCD_init();					//Initialize LCD
	
	//-----------------------------------
	// Save custom characters
	//-----------------------------------
	LCD_command(0x40);			//Set cursor to the beginning
								//of CGRAM address
	//#0: Smiley
	LCD_senddata(0b00000);
	LCD_senddata(0b01010);
	LCD_senddata(0b01010);
	LCD_senddata(0b00000);
	LCD_senddata(0b10001);
	LCD_senddata(0b01110);
	LCD_senddata(0b00000);
	LCD_senddata(0b00000);

	//#1: Sad
	LCD_senddata(0b00000);
	LCD_senddata(0b01010);
	LCD_senddata(0b01010);
	LCD_senddata(0b00000);
	LCD_senddata(0b01110);
	LCD_senddata(0b10001);
	LCD_senddata(0b00000);
	LCD_senddata(0b00000);

	//#2: Battery
	LCD_senddata(0b01110);
	LCD_senddata(0b11011);
	LCD_senddata(0b10001);
	LCD_senddata(0b10001);
	LCD_senddata(0b10001);
	LCD_senddata(0b11111);
	LCD_senddata(0b11111);
	LCD_senddata(0b11111);

	//#3: Love
	LCD_senddata(0b00000);
	LCD_senddata(0b01010);
	LCD_senddata(0b10101);
	LCD_senddata(0b10001);
	LCD_senddata(0b01010);
	LCD_senddata(0b00100);
	LCD_senddata(0b00000);
	LCD_senddata(0b00000);

	//#4: Cherry, apple...or bom?!
	LCD_senddata(0b00010);
	LCD_senddata(0b00100);
	LCD_senddata(0b00100);
	LCD_senddata(0b01110);
	LCD_senddata(0b10001);
	LCD_senddata(0b10001);
	LCD_senddata(0b10001);
	LCD_senddata(0b01110);

	//#5:Big '!'
	LCD_senddata(0b01110);
	LCD_senddata(0b01110);
	LCD_senddata(0b01110);
	LCD_senddata(0b01110);
	LCD_senddata(0b01110);
	LCD_senddata(0b00000);
	LCD_senddata(0b01110);
	LCD_senddata(0b01110);

	//#6: 90 degree rotated 'RH'
	LCD_senddata(0b11111);
	LCD_senddata(0b00100);
	LCD_senddata(0b11111);
	LCD_senddata(0b00000);
	LCD_senddata(0b01001);
	LCD_senddata(0b10101);
	LCD_senddata(0b10110);
	LCD_senddata(0b11111);

	//#7: 90 degree rotated '2T'
	LCD_senddata(0b10000);
	LCD_senddata(0b11111);
	LCD_senddata(0b10000);
	LCD_senddata(0b00000);
	LCD_senddata(0b01001);
	LCD_senddata(0b10101);
	LCD_senddata(0b10011);
	LCD_senddata(0b01001);

	LCD_command(0x01);	//Clear display

	//-----------------------------------
	// Display custom characters
	//-----------------------------------
	LCD_command(0x80);		//Set cursor to line 1, position 0
	LCD_senddata(7);		//Display pattern #7 = 2T

	LCD_command(0xC0);		//Set cursor to line 2, position 0
	LCD_senddata(6);		//Display pattern #6 = RH

	//-----------------------------------
	// Display ALL custom characters
	//-----------------------------------
	for(i=0;i<8;i++)		//loop 8 times
	{							
		LCD_command(0x83+i);//Set cursor to line 1, position 3+i
		LCD_senddata(i);	//Display 'i'th custom character
	}

	//-----------------------------------
	// End of program
	//-----------------------------------
	while(1);				//Infinite loop
}


//===============================================================================
//	Functions
//===============================================================================
void LCD_init()
{
	LCD_data = 0x38;     	//Function set: 2 Line, 8-bit, 5x7 dots
	LCD_rs   = 0;        	//Selected command register
	LCD_rw   = 0;        	//We are writing in data register
	LCD_en   = 1;        	//Enable H->L
	LCD_en   = 0;
	LCD_busy();          	//Wait for LCD to process the command
	LCD_data = 0x0F;     	//Display on, Cursor blinking command
	LCD_rs   = 0;        	//Selected command register
	LCD_rw   = 0;        	//We are writing in data register
	LCD_en   = 1;        	//Enable H->L
	LCD_en   = 0;
	LCD_busy();          	//Wait for LCD to process the command
	LCD_data = 0x01;     	//Clear LCD
	LCD_rs   = 0;        	//Selected command register
	LCD_rw   = 0;        	//We are writing in data register
	LCD_en   = 1;        	//Enable H->L
	LCD_en   = 0;
	LCD_busy();          	//Wait for LCD to process the command
	LCD_data = 0x06;     	//Entry mode, auto increment with no shift
	LCD_rs   = 0;        	//Selected command register
	LCD_rw   = 0;        	//We are writing in data register
	LCD_en   = 1;        	//Enable H->L
	LCD_busy();
}

void LCD_busy()
{
	TRISD  = 0xFF;          //PORTD as i/p
	LCD_en   = 1;           //
	LCD_rs   = 0;           //Selected command register
	LCD_rw   = 1;           //We are reading
	while(LCD_DB7)			//read busy flag again and again 
	{						//till it becomes 0
		LCD_en   = 0;     	//Enable H->L
		LCD_en   = 1;
	}
	TRISD  = 0x00;          //PORTD as o/p
}

//Routine to send command to LCD
void LCD_command(unsigned char var)
{
     LCD_data = var;      	//Function set: 2 lines, 8-bit, 5x7 dots
     LCD_rs   = 0;        	//Selected command register
     LCD_rw   = 0;        	//We are writing in instruction register
     LCD_en   = 1;        	//Enable H->L
     LCD_en   = 0;
     LCD_busy();          	//Wait for LCD to process the command
}

//Routine to send data (single character) to LCD
void LCD_senddata(unsigned char var)
{
     LCD_data = var;      //Function set: 2 lines, 8-bit, 5x7 dots
     LCD_rs   = 1;        //Selected data register
     LCD_rw   = 0;        //We are writing
     LCD_en   = 1;        //Enable H->L
     LCD_en   = 0;
     LCD_busy();          //Wait for LCD to process the command
}

//Routine to send string to LCD
void LCD_sendstring(const char *var)
{
    while (*var)				//till string ends
     	LCD_senddata(*var++);  	//send characters one by one
}
