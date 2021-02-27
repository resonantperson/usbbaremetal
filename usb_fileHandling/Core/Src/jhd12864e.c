/*
 * jhd12864e.c
 *
 *  Created on: Feb 9, 2021
 *      Author: praveen
 */

#include "jhd12864e.h"
extern TIM_HandleTypeDef htim1;

GLCD_t glcd_handle={0};

static void GLCD_Initialize_handle(void);
static void GLCD_segmentSelect(chip_select);
static void GLCD_segmentToggle(void);
static void GLCD_writeDataLine(cmd_or_data, uint8_t);
static void config_GLCD(void);
static void delay_us(uint16_t us);

const char font[][5] =
{
 {0x00, 0x00, 0x00, 0x00, 0x00} // 20 (Space) 
,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ?
,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j 
,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ->
,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f <-
};
/* //right now not in use
const uint8_t splashscreen[1024] = {
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  63, 255,  63, 255,  63, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   0, 255,   0, 255,   0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 171, 171, 171, 171, 171, 171, 171, 171, 171, 171, 128, 235, 224, 251, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,   7,   7,   7, 255, 255,  31,   7,   7, 199,   7,   7,  31, 255, 255, 255,  31,  15,   7, 231, 231, 199,   7,  15,  31, 255, 255,   7,   7,   7, 255, 255, 255,   7,   7,   7, 255, 255,  31,   7,   7, 199,   7,   7,  31, 255, 255, 255,   7,   7,   7, 103, 103,   7,   7,  15, 255, 255,   7,   7,   7, 231, 231,   7,  15,  31, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 249, 241, 225, 231, 231, 224, 240, 248, 255, 227, 224, 224, 248, 249, 248, 224, 224, 227, 255, 255, 248, 240, 224, 231, 225, 227, 224, 224, 200, 255, 255, 248, 240, 224, 231, 231, 231, 224, 240, 248, 255, 227, 224, 224, 248, 249, 248, 224, 224, 227, 255, 255, 224, 224, 224, 254, 252, 240, 225, 227, 239, 255, 224, 224, 224, 231, 231, 224, 240, 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};
*/
static const char cursor[5]={0x7F,0x3E,0x1C,0x08,0x00};

void GLCD_Init(void)	// GLCD Initialization function
{
	GLCD_Initialize_handle(); //
		rst_high();                                   // Make reset pin High 
    GLCD_segmentSelect(glcd_handle.current_segment); //Select Left half and right half of display 
    HAL_Delay(20);																//MSdelay(20);
    GLCD_SendCommand(GLCD_CMD_OFF);		            // Display OFF 
    GLCD_SendCommand(glcd_handle.current_column);	// Set Y address (column=0) 
    GLCD_SendCommand(glcd_handle.current_page);		// Set x address (page=0) 
    GLCD_SendCommand(glcd_handle.start_line);		  // Set z address (start line=0) 
    GLCD_SendCommand(GLCD_CMD_ON);		            // Display ON
	glcd_handle.current_segment=left_segment;
}

static void GLCD_Initialize_handle(void)
{
	glcd_handle.current_column = 0;
	glcd_handle.current_page = 0;
	glcd_handle.current_segment = both_segment;
	glcd_handle.start_line = 0;
	glcd_handle.text_buf = NULL;
}

static void GLCD_segmentSelect(chip_select config)
{
	switch(config) {
		case left_segment: {cs1_high();cs2_low(); break;}
		case right_segment: {cs1_low();cs2_high(); break;}
		case both_segment:  {cs1_high();cs2_high(); break;}
	}
}

static void GLCD_segmentToggle(void)
{
	if(glcd_handle.current_segment == left_segment)
		glcd_handle.current_segment = right_segment;
	else
		glcd_handle.current_segment = left_segment;
		
	GLCD_segmentSelect(glcd_handle.current_segment);
}

static void GLCD_writeDataLine(cmd_or_data flag, uint8_t value)
{
	port_data(value);	      // Copy command or data to the LCD data pins 
	if(flag == cmd_mode)
		rs_low();			        // Make RS LOW for command register
	else
		rs_high();		        // Make RS HIGH for data register
	
	en_high();              // Make HIGH-LOW transition on Enable
	delay_us(2);
	en_low();
	delay_us(2);
}

static void config_GLCD(void)
{
	GLCD_SendCommand(GLCD_CMD_ON);
	GLCD_SendCommand(GLCD_CMD_COL0 + glcd_handle.current_column);
	GLCD_SendCommand(GLCD_CMD_PAGE0 + glcd_handle.current_page);
	GLCD_SendCommand(GLCD_CMD_STRT_LINE + glcd_handle.start_line);
	GLCD_segmentSelect(glcd_handle.current_segment);
}
static void delay_us(uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < us);  // wait for the counter to reach the us input in the parameter
}

void GLCD_Clear_All(void)
{
	glcd_handle.current_page=0;
	glcd_handle.current_column=0;
	glcd_handle.current_segment=both_segment;

	for(;glcd_handle.current_page<8;glcd_handle.current_page++)
	{
		config_GLCD();
		for(;glcd_handle.current_column<64;glcd_handle.current_column++)
			GLCD_SendData(0);
		
		glcd_handle.current_column=0;
	}

	glcd_handle.current_page=0;
	GLCD_SendCommand(GLCD_CMD_COL0 + glcd_handle.current_column);
	GLCD_SendCommand(GLCD_CMD_PAGE0 + glcd_handle.current_page);
}

void GLCD_printLine(uint8_t page_num, char* str) //printing the data in the corresponding page
{
	glcd_handle.current_page= page_num;
	glcd_handle.current_column = 0;
	glcd_handle.current_segment = left_segment;
	glcd_handle.text_buf = str;
	
	uint16_t max_chars=strlen(glcd_handle.text_buf), char_column = 0;
	/* char_column' controls the column data of a character */

	for(uint8_t count=0; count<2; count++)		//run once for left-half, once for right-half
	{
		if(glcd_handle.current_segment == left_segment)
			glcd_handle.current_column = 5;

		config_GLCD();
		while(glcd_handle.current_column<64 && ((char_column/5)<max_chars))
		{
			//print 1 column of the selected page at a time
			GLCD_SendData(font[(glcd_handle.text_buf[char_column/5] - 32)][char_column%5]);
			
			if(char_column%5 == 4)
			{	
				glcd_handle.current_column++;
				GLCD_SendCommand(GLCD_CMD_COL0 + glcd_handle.current_column);
				GLCD_SendData(0x00);
			}
			glcd_handle.current_column++; char_column++;
		}
		GLCD_segmentToggle();		// select the right segment
		glcd_handle.current_column = 0;
		
		//GLCD_SendCommand(GLCD_CMD_COL0);
	}
	//glcd_handle.current_page=0;
}
/*           Problem found
Problems in GLCD_printLine we have worked on spacing issues with
some logics but when it tries to print the data in right the start of the char missing the space
Note : if it is the new char only then we need to do space
If it is the space char we don't need space
If it lenght exceeds need to stop before the start of letter(Not neccessary)
*/


// Need to create a pop up function in order to print for certain notification and returns to orginal state
// In this function previous state of glcd is saved so that glcd_handle will return back to state

void GLCD_Cursor_update(cursor_modify value)
{
	glcd_handle.current_page = glcd_handle.cursor_pos;	//for clearing prev cursor position
	glcd_handle.current_column=0;
	glcd_handle.current_segment=left_segment;
	config_GLCD();
	for(;glcd_handle.current_column<5;glcd_handle.current_column++)
		GLCD_SendData(0);

	if(value == clr)	// return if only clearing the cursor
	{
		glcd_handle.current_column=0;
		return;
	}

	switch(value) {
		case incr: glcd_handle.cursor_pos++; break;
		case decr: glcd_handle.cursor_pos--; break;
		default: break;
	}

	glcd_handle.current_page = glcd_handle.cursor_pos;
	glcd_handle.current_column=0;
	config_GLCD();
	for(;glcd_handle.current_column<5;glcd_handle.current_column++)
		GLCD_SendData(cursor[glcd_handle.current_column]);

	glcd_handle.current_column=0;
}
