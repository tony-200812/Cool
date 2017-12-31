#include "stm32f10x.h"
#include "config.h"
#include "lcd.h"
#include "voice.h"


extern struct __voice_file voice_file;

int main(void)
{
	u32 temp = 0;
	
	STM32_CONFIG();
	lcd_init();
	play_voice(voice_file);
	
	while (1)
	{
		temp += 3000;
		if (temp >= 17000)
			temp = 0;
		
		delay_nms(1000);
		
		TIM2->CCR2  = temp;
	}
}
