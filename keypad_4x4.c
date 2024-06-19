#include "keypad_4x4.h"

#define IDLE_OUTPUT_KEY 16

typedef enum{
	PIN_LOW  = 0,
	PIN_HIGH = 1,
}PinState;

uint8_t  keypad_value_detect (void);
uint8_t is_any_key_pressed (void);
void keypad_update (void);
void write_row (uint8_t value);
GPIO_PinState read_col (uint8_t col);


static uint8_t g_output_key = 0;
static uint8_t g_output_key_prior = 0;
static uint8_t g_keypad_row  = MAX_KEYPAD_ROW;
static uint8_t g_keypad_col  = MAX_KEYPAD_COL;
/*static*/ uint8_t kp            = 0;
static keypad_key_state_t trigge_state = FALLING_EDGE;

uint32_t number = 0;

void keypad_kp_0_9_func (void){
	char lcd_buff[17] = {0};		
	number = (number * 10) + get_keypad_value();  //for exampel :)
	sprintf(lcd_buff, "%d", number);
	LCD_Puts(0, 0, lcd_buff);
}
void keypad_kp_10_func (void){ //kp A   //MENU_CHANGE_PF
	number = 0;
	LCD_Clear();
}
void keypad_kp_11_func (void){ //kp B   //MENU_VIEW_DETALES   //set MOTOR_A PF

}
void keypad_kp_12_func (void){ //kp C   //MENU_RESET_PRODUCTS_NUM
	
}
void keypad_kp_13_func (void){ //kp D  //MENU_MAIN_PAGE
	
}

void keypad_kp_14_func (void){
	
}
void keypad_kp_15_func (void){
	
}

void keypad_process (void){
	keypad_update();
	kp = keypad_value_detect();
	
	if(keypad_state_detect() == trigge_state)
	{
		
		switch(get_keypad_value())
		{	
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			    keypad_kp_0_9_func();
			break;
			case 10:
				keypad_kp_10_func();
			break;
			case 11:
				keypad_kp_11_func();
			break;
			case 12:
				keypad_kp_12_func();
			break;
			case 13:
				keypad_kp_13_func();
			break;
			case 14:
				keypad_kp_14_func();
			break;
			case 15:
				keypad_kp_15_func();
			break;
			default:
			break;
		}
	}
}
void keypad_init (keypad_key_state_t state){
	trigge_state = state;
}
void write_row (uint8_t value){
	HAL_GPIO_WritePin(KEYPAD_R0_GPIO_Port, KEYPAD_R0_Pin, (GPIO_PinState)((value & 0x01) == 0));
	HAL_GPIO_WritePin(KEYPAD_R1_GPIO_Port, KEYPAD_R1_Pin, (GPIO_PinState)((value & 0x02) == 0));
	HAL_GPIO_WritePin(KEYPAD_R2_GPIO_Port, KEYPAD_R2_Pin, (GPIO_PinState)((value & 0x04) == 0));
	HAL_GPIO_WritePin(KEYPAD_R3_GPIO_Port, KEYPAD_R3_Pin, (GPIO_PinState)((value & 0x08) == 0));
}
GPIO_PinState read_col (uint8_t col){
	GPIO_PinState tmp = GPIO_PIN_SET;
	switch(col)
	{
		case 0:
			tmp = HAL_GPIO_ReadPin(KEYPAD_C0_GPIO_Port, KEYPAD_C0_Pin);
		break;
		case 1:
			tmp = HAL_GPIO_ReadPin(KEYPAD_C1_GPIO_Port, KEYPAD_C1_Pin);
		break;
		case 2:
			tmp = HAL_GPIO_ReadPin(KEYPAD_C2_GPIO_Port, KEYPAD_C2_Pin);
		break;
		case 3:
			tmp = HAL_GPIO_ReadPin(KEYPAD_C3_GPIO_Port, KEYPAD_C3_Pin);
		break;
	}
	
	return tmp;
}
uint8_t check_cols (void){  //get all column state in an uint8_t data
	uint8_t col_data = 0;
	
	col_data |= ((uint8_t)HAL_GPIO_ReadPin(KEYPAD_C0_GPIO_Port, KEYPAD_C0_Pin) << 0x00);
	col_data |= ((uint8_t)HAL_GPIO_ReadPin(KEYPAD_C1_GPIO_Port, KEYPAD_C1_Pin) << 0x01);
	col_data |= ((uint8_t)HAL_GPIO_ReadPin(KEYPAD_C2_GPIO_Port, KEYPAD_C2_Pin) << 0x02);
	col_data |= ((uint8_t)HAL_GPIO_ReadPin(KEYPAD_C3_GPIO_Port, KEYPAD_C3_Pin) << 0x03);
	
	return col_data;
}
uint8_t get_keypad_value (void){
	return kp;
}
uint8_t  keypad_value_detect (void){
static const uint8_t key_matrix[MAX_KEYPAD_ROW][MAX_KEYPAD_COL] = 
{
    { 1, 2,  3, 10},
    { 4, 5,  6, 11},
    { 7, 8,  9, 12},
    {14, 0, 15, 13}
};
 
    if ((g_keypad_col < MAX_KEYPAD_COL) && (g_keypad_row < MAX_KEYPAD_ROW))
    {
        g_output_key = key_matrix[g_keypad_row][g_keypad_col];    
    }
    else
    {
        g_output_key = IDLE_OUTPUT_KEY;
		// txSendDataLen("valDec", strlen("valDec"));
		// _delay_ms(5);  
    }                 
    
    return g_output_key;
}
void keypad_scan (void){																						
uint8_t i = 0;
uint8_t j = 0;

	for(i = 0; i <= MAX_KEYPAD_ROW; i++)
	{
		write_row(1 << i);
		for(j = 0; j <= MAX_KEYPAD_COL; j++)
		{
			if(read_col(j) == PIN_LOW)
			{
				g_keypad_row = i;
				g_keypad_col = j;
			}
		}
	}
}

uint8_t is_any_key_pressed (void){
uint8_t output_state = 0;
  
    write_row(0x0F);
	if(check_cols() != 0x0F){ 
        output_state = 1;
    }
    else{
        output_state = 0;
    }
    
    return output_state;   
}

void keypad_update (void){
 
    if (is_any_key_pressed()){     
        keypad_scan();
    }
    else{
        g_keypad_row = MAX_KEYPAD_ROW;
        g_keypad_col = MAX_KEYPAD_COL;
    }
}

keypad_key_state_t keypad_state_detect (void){
keypad_key_state_t key_state = HIGH_LEVEL;

	if(g_output_key == g_output_key_prior)
    {
        if(g_output_key == IDLE_OUTPUT_KEY)
        {
            key_state = HIGH_LEVEL;    
        }
        else
        {
            key_state = LOW_LEVEL;  
        }    
    }
    else
    {   
        g_output_key_prior = g_output_key;
        
        if (g_output_key == IDLE_OUTPUT_KEY)
        {
            key_state = RISING_EDGE;  
        }
        else
        {
            key_state = FALLING_EDGE;
			// txSendDataLen("statDec", strlen("statDec"));
			// _delay_ms(20);   
        }    
    }
    
    return key_state;
}
////////////////
