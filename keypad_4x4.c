#include "keypad_4x4.h"


#define IDLE_OUTPUT_KEY 16

uint8_t  keypad_value_detect (void);
uint8_t is_any_key_pressed (void);
void keypad_update (void);
uint8_t timeout_is_ok(uint32_t ms);

static uint8_t g_output_key = 0;
static uint8_t g_output_key_prior = 0;
static uint8_t g_keypad_row  = MAX_KEYPAD_ROW;
static uint8_t g_keypad_col  = MAX_KEYPAD_COL;
static uint8_t kp            = 0;
static keypad_key_state_t trigge_state = FALLING_EDGE;




__weak void keypad_kp_0_9_func (void)
{
	UNUSED(0); //just for save this func
}
__weak void keypad_kp_10_func (void)
{
	UNUSED(10); //just for save this func
}
__weak void keypad_kp_11_func (void)
{
	UNUSED(11); //just for save this func
}
__weak void keypad_kp_12_func (void)
{
	UNUSED(12); //just for save this func
}
__weak void keypad_kp_13_func (void)
{
	UNUSED(13); //just for save this func
}
__weak void keypad_kp_14_func (void)
{
	UNUSED(14); //just for save this func
}
__weak void keypad_kp_15_func (void)
{
	UNUSED(15); //just for save this func
}

void keypad_process (void)
{
	keypad_update();
	kp = keypad_value_detect();
	
	if(keypad_state_detect() == trigge_state)
	{
		
		switch(get_keypad_value())
		{
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
				keypad_kp_0_9_func();
			break;
		}
	}
}
void keypad_init (keypad_key_state_t state)
{
	trigge_state = state;
}

void write_row (uint8_t value)
{
	HAL_GPIO_WritePin(KEYPAD_R0_GPIO_Port, KEYPAD_R0_Pin, (GPIO_PinState)((value & 0x01) == 0));
	HAL_GPIO_WritePin(KEYPAD_R1_GPIO_Port, KEYPAD_R1_Pin, (GPIO_PinState)((value & 0x02) == 0));
	HAL_GPIO_WritePin(KEYPAD_R2_GPIO_Port, KEYPAD_R2_Pin, (GPIO_PinState)((value & 0x04) == 0));
	HAL_GPIO_WritePin(KEYPAD_R3_GPIO_Port, KEYPAD_R3_Pin, (GPIO_PinState)((value & 0x08) == 0));
}
GPIO_PinState read_col (uint8_t col)
{
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
uint8_t check_cols (void)  //get all column state in an uint8_t data
{
	uint8_t col_data = 0;
	
	col_data |= ((uint8_t)HAL_GPIO_ReadPin(KEYPAD_C0_GPIO_Port, KEYPAD_C0_Pin) << 0x00);
	col_data |= ((uint8_t)HAL_GPIO_ReadPin(KEYPAD_C1_GPIO_Port, KEYPAD_C1_Pin) << 0x01);
	col_data |= ((uint8_t)HAL_GPIO_ReadPin(KEYPAD_C2_GPIO_Port, KEYPAD_C2_Pin) << 0x02);
	col_data |= ((uint8_t)HAL_GPIO_ReadPin(KEYPAD_C3_GPIO_Port, KEYPAD_C3_Pin) << 0x03);
	
	return col_data;
}
uint8_t get_keypad_value (void)
{
	return kp;
}
uint8_t  keypad_value_detect (void)
{
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
    }                 
    
    return g_output_key;
}
void keypad_scan (void)
{																						
uint8_t i = 0;
uint8_t j = 0;

	for(i = 0; i <= MAX_KEYPAD_ROW; i++)
	{
		write_row(1 << i);
		for(j = 0; j <= MAX_KEYPAD_COL; j++)
		{
			if(read_col(j) == GPIO_PIN_RESET)
			{
				g_keypad_row = i;
				g_keypad_col = j;
			}
		}
	}
}
uint8_t timeout_is_ok(uint32_t ms){
	volatile static uint32_t currTick = 0;
	volatile static uint32_t nextTick = 0;
	uint8_t retVal = 0;
	currTick = HAL_GetTick();
	if(nextTick > currTick){
		nextTick = currTick + ms;
		retVal = 1;
	}
	return retVal;
}
uint8_t is_any_key_pressed (void)
{
uint8_t output_state = 0;
  
    write_row(0x0F);
		if(check_cols() != 0x0F)
    { 
        if(timeout_is_ok(200))output_state = 1;
    }
    else
    {
        output_state = 0;
    }
    
    return output_state;   
}

void keypad_update (void)
{   
 
    if (is_any_key_pressed())
    {     
        keypad_scan();
    }
    else
    {
        g_keypad_row = MAX_KEYPAD_ROW;
        g_keypad_col = MAX_KEYPAD_COL;	
    }
}

keypad_key_state_t keypad_state_detect (void)
{
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
        }    
    }
    
    return key_state;
}
////////////////
