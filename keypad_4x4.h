/*
for initialize use this function:
	"keypad_init(keypad_key_state_t state);"

and use "keypad_process();" in the while(1).
	
and notice that you must define this func for use keypad:
	void keypad_kp_0_9_func (void);
	void keypad_kp_10_func  (void);
	void keypad_kp_11_func  (void);
	void keypad_kp_12_func  (void);
	void keypad_kp_13_func  (void);
	void keypad_kp_14_func  (void);
	void keypad_kp_15_func  (void);

and this example recommended(with use "LCD.h"):
	void keypad_kp_0_9_func (void)
	{
		char lcd_buff[17] = {0};
		
		number = (number * 10) + get_keypad_value();  //for exampel :)
		sprintf(lcd_buff, "%d", number);
		LCD_Puts(0, 0, lcd_buff);
	}
	void keypad_kp_10_func (void)
	{
		number = 0;
		LCD_Clear();
	}

*/


#ifndef __KEYPAD_4X4_H
#define __KEYPAD_4X4_H

#include "main.h"
#include "gpio.h"
#include "lcd.h"
#include "stdio.h"

#define MAX_KEYPAD_ROW	4
#define MAX_KEYPAD_COL	4

#define KEYPAD_TIMEOUT 0 //ms

typedef enum
{
	LOW_LEVEL,
	HIGH_LEVEL,
	FALLING_EDGE,
	RISING_EDGE
}keypad_key_state_t;

void keypad_kp_0_9_func(void);
void keypad_kp_10_func(void);
void keypad_kp_11_func(void);
void keypad_kp_12_func(void);
void keypad_kp_13_func(void);
void keypad_kp_14_func(void);
void keypad_kp_15_func(void);

void keypad_process (void);
uint8_t get_keypad_value (void);
void keypad_init (keypad_key_state_t state);
keypad_key_state_t keypad_state_detect (void);

#endif //__KEYPAD_4X4_H
