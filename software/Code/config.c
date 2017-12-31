#include "stm32f10x.h"


/*******************************************************************************
 函 数 名：	RCC_CONFIG
 功能描述：	使能外设时钟
 入口参数：	无
 出口参数：	无
********************************************************************************/
void RCC_CONFIG(void)
{
	RCC->APB2ENR = RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN |
				   RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN |
				   RCC_APB2ENR_IOPEEN | RCC_APB2ENR_AFIOEN;
				  
	RCC->APB1ENR = RCC_APB1ENR_TIM4EN | RCC_APB1ENR_DACEN  |
				   RCC_APB1ENR_TIM2EN;

	RCC->AHBENR  = RCC_AHBENR_FSMCEN;	
}

/*******************************************************************************
 函 数 名：	GPIO_CONFIG
 功能描述：	GPIO 配置
 入口参数：	无
 出口参数：	无
********************************************************************************/
void GPIO_CONFIG(void)
{
	GPIOA->CRL = GPIO_CRL_CNF1_1  | GPIO_CRL_MODE1  |		//LED_PWM
				 GPIO_CRL_MODE3   | 						//LED
				 GPIO_CRL_CNF4_1  | GPIO_CRL_MODE4;			//DAC
				  
	GPIOA->CRH = GPIO_CRH_CNF15_1 | GPIO_CRH_MODE15;		//SPI3_NSS

	GPIOB->CRL = GPIO_CRL_CNF3_1  | GPIO_CRL_MODE3	|		//SPI3_SCK
			     GPIO_CRL_CNF3_1  | 						//SPI3_MISO
			     GPIO_CRL_CNF5_1  | GPIO_CRL_MODE5;			//SPI3_MOSI
			     
	GPIOB->CRH = GPIO_CRH_MODE10  |							//LCD_RST
				 GPIO_CRH_CNF12_1 | GPIO_CRH_MODE12 |		//FLASH_CS
				 GPIO_CRH_CNF13_1 | GPIO_CRH_MODE13 |		//FLASH_CLK
				 GPIO_CRH_CNF14_0 |							//FLASH_MISO
				 GPIO_CRH_CNF15_1 | GPIO_CRH_MODE15;		//FLASH_MOSI

	GPIOC->CRL = GPIO_CRL_MODE0 |
			     GPIO_CRL_MODE1 |
			     GPIO_CRL_MODE2 |
			     GPIO_CRL_MODE3 |
			     GPIO_CRL_MODE4 |
			     GPIO_CRL_MODE5 |
			     GPIO_CRL_MODE6 |
			     GPIO_CRL_MODE7;

	GPIOC->CRH = GPIO_CRH_CNF10_1 | GPIO_CRH_MODE10 |		//UART4_TX
				 GPIO_CRH_CNF11_0;							//UART4_RX

	GPIOD->CRL = GPIO_CRL_CNF0_1  | GPIO_CRL_MODE0  |		//D02
				 GPIO_CRL_CNF1_1  | GPIO_CRL_MODE1  |		//D03
				 GPIO_CRL_CNF4_1  | GPIO_CRL_MODE4  |		//RW
				 GPIO_CRL_CNF5_1  | GPIO_CRL_MODE5  |		//WR
				 GPIO_CRL_CNF7_1  | GPIO_CRL_MODE7;			//CS
	
	GPIOD->CRH = GPIO_CRH_CNF8_1  | GPIO_CRH_MODE8  |		//D13
				 GPIO_CRH_CNF9_1  | GPIO_CRH_MODE9  |		//D14
				 GPIO_CRH_CNF10_1 | GPIO_CRH_MODE10 |		//D15
				 GPIO_CRH_CNF11_1 | GPIO_CRH_MODE11 |		//RS
				 GPIO_CRH_CNF14_1 | GPIO_CRH_MODE14 |		//D00
				 GPIO_CRH_CNF15_1 | GPIO_CRH_MODE15;		//D01
	
	GPIOE->CRL = GPIO_CRL_CNF7_1  | GPIO_CRL_MODE7;			//D04
	
	GPIOE->CRH = GPIO_CRH_CNF8_1  | GPIO_CRH_MODE8  |		//D05
				 GPIO_CRH_CNF9_1  | GPIO_CRH_MODE9  |		//D06
				 GPIO_CRH_CNF10_1 | GPIO_CRH_MODE10 |		//D07
				 GPIO_CRH_CNF11_1 | GPIO_CRH_MODE11 |		//D08
				 GPIO_CRH_CNF12_1 | GPIO_CRH_MODE12 |		//D09
				 GPIO_CRH_CNF13_1 | GPIO_CRH_MODE13 |		//D10
				 GPIO_CRH_CNF14_1 | GPIO_CRH_MODE14 |		//D11
				 GPIO_CRH_CNF15_1 | GPIO_CRH_MODE15;		//D12

	AFIO->MAPR = 2 << 24;	//SWD
	
	GPIOA->BSRR = 1 << 1;
	GPIOB->BSRR = 1 << 10;
	GPIOD->BSRR = 1 << 4 | 1 << 5 | 1 << 7 | 1 << 11;
}

/*******************************************************************************
 函 数 名：	DAC1_CONFIG
 功能描述：	DAC1 配置
 入口参数：	无
 出口参数：	无
********************************************************************************/
void DAC1_CONFIG(void)
{
	DAC->CR  = 0x83C;
	DAC->CR |= 1;			//DAC1使能
	
	DAC->DHR12R1  = 0;		
	DAC->SWTRIGR |= 0x01;	//软件启动转换
}

/*******************************************************************************
 函 数 名：	TIM4_Config
 功能描述：	TIM4 配置 触发DAC1转换
 入口参数：	无
 出口参数：	无
********************************************************************************/
void TIM4_CONFIG(void)
{
 	TIM4->ARR   = 125;
	TIM4->PSC   = 72;  					//预分频器
	
	TIM4->DIER |= 1 << 0;   			//允许更新中断				
	TIM4->DIER |= 1 << 6;   			//允许触发中断
		  							
	TIM4->CR1 |= 0x01;    				//使能定时器4
	
  	NVIC->IP[TIM4_IRQn] = 0x00;        //中断优先级
    NVIC->ISER[TIM4_IRQn >> 0x05] = (u32)0x01 << (TIM4_IRQn & (u8)0x1F);//	
}

/*******************************************************************************
 函 数 名：	USART2_CONFIG
 功能描述：	USART2引脚配置 (串口,19200bps 1位开始位，1位结束位，9位数据位)
 入口参数：	无
 出口参数：	无
********************************************************************************/
void USART2_CONFIG(void)
{
	USART2->CR1 = 0;
	USART2->CR1 = 1 << 12 | 1 << 6 | 1 << 5 | 1 << 3 | 1 << 2;	
	USART2->BRR = 0x0753;  		
	USART2->CR2 = 0 << 14 | 0 << 12 | 0 << 11;
	USART2->CR3 = 0;
	USART2->CR1 |= 1 << 13;	
	
	NVIC->IP[USART2_IRQn] = 0x20;		//中断优先级
	NVIC->ISER[USART2_IRQn >> 0x05] = (u32)0x01 << (USART2_IRQn & (u8)0x1F);//开中断	
}

/*******************************************************************************
 函 数 名：	USART3_CONFIG
 功能描述：	USART3引脚配置 (语音)
 入口参数：	无
 出口参数：	无
********************************************************************************/
void USART3_CONFIG(void)
{
	USART3->CR1  = 0;
	USART3->CR1 |= 1 << 3;	
	USART3->BRR  = 0x0EA6;  // 9600bps //0x0b00;
	USART3->CR2  = 0 << 14 | 0 << 12 | 1 << 11 | 0 << 10 | 
	   			   3 << 9 | 1 << 8 | 0 << 6;
	USART3->CR3  = 0;
	USART3->CR1 |= 1 << 13;	
}

/*******************************************************************************
 函 数 名：	SPI1_CONFIG
 功能描述：	SPI1 配置 (FLASH)
 入口参数：	无
 出口参数：	无
********************************************************************************/
void SPI1_CONFIG(void)
{
    SPI1->CR1 &= ~0x0040;
	SPI1->CR1 = 0x0307;
	//SPI1->CR1 = 0x0304;
	SPI1->I2SCFGR &= 0xf7ff;
	SPI1->CRCPR = 7;
    SPI1->CR1 |= 0x0040;
}

/*******************************************************************************
 函 数 名：	SPI2_CONFIG
 功能描述：	SPI2 配置 (fm1702)
 入口参数：	无
 出口参数：	无
********************************************************************************/
void SPI2_CONFIG(void)
{
    SPI2->CR1 &= ~0x0040;
	SPI2->CR1 = 0x0314;
	SPI2->I2SCFGR &= 0xf7ff;
	SPI2->CRCPR = 7;
    SPI2->CR1 |= 0x0040;
}

/*******************************************************************************
 函 数 名：	EXTI1_CONFIG
 功能描述：	外部中断配置(掉电中断)
 入口参数：	无
 出口参数：	无
********************************************************************************/
void EXTI1_CONFIG(void)
{
	AFIO->EXTICR[0] = 2 << 4;

	EXTI->IMR  |=  1 << 1;
	EXTI->FTSR |= 1 << 1;

    NVIC->IP[EXTI1_IRQn] = 0x00;
    NVIC->ISER[EXTI1_IRQn >> 0x05] = 
      (u32)0x01 << (EXTI1_IRQn & (u8)0x1F);
}

/*******************************************************************************
 函 数 名：	EXTI7_CONFIG
 功能描述：	外部中断配置(fm1702中断)
 入口参数：	无
 出口参数：	无
********************************************************************************/
void EXTI7_CONFIG(void)
{
	AFIO->EXTICR[1] = 2 << 12;

	EXTI->IMR  |= 1 << 7;
	EXTI->FTSR |= 1 << 7;

    NVIC->IP[EXTI9_5_IRQn] = 0x20;
    NVIC->ISER[EXTI9_5_IRQn >> 0x05] = 
      (u32)0x01 << (EXTI9_5_IRQn & (u8)0x1F);
}

/*******************************************************************************
 函 数 名：	TIM2_CONFIG
 功能描述：	TIM2 初始化 20ms中断一次 中断优先级 3 
 入口参数：	无
 出口参数：	无
********************************************************************************/
void TIM2_CONFIG(void)
{	
	u32 temp;
	
	temp = SystemCoreClock / 4 / 1000;	
	
	TIM2->CR1   = 0;
	TIM2->PSC   = 3;	  					//预分频值
	TIM2->RCR   = 0;    					//重复记数值,下次脉冲有效
	TIM2->EGR   = 1 << 0;					//重新初始化寄存器
	TIM2->CCER &= ~(1 << 0); 				//高电平有效，其他通道打开
	TIM2->CR2   = 0;     					//没用
	TIM2->CR1  |= 0x0080;  					//值装入缓存器
	TIM2->CCMR1 = (1 << 11) | (7 << 12);  	//配置通道4
	
	TIM2->ARR   = temp;
	TIM2->CCR2  = temp;

	TIM2->CCER  = (1 << 4) | (1 << 5);  	//使能输出
	TIM2->CR1  |= 1 << 0;
}

/*******************************************************************************
 函 数 名：	IWDG_CONFIG
 功能描述：	独立看门狗初始化
 入口参数：	无
 出口参数：	无
********************************************************************************/
void IWDG_CONFIG(void)
{
	IWDG->KR  = 0x5555; 
	IWDG->PR  = 0x04;	//64分频
	IWDG->RLR = 0x780;	//3s 看门狗中断
	IWDG->KR  = 0xCCCC;	//启动看门狗
	IWDG->KR  = 0xAAAA; //喂狗
}


/*******************************************************************************
 函 数 名：	FSMC_Config
 功能描述：	LCD总线接口配置
 入口参数：	无
 出口参数：	无
********************************************************************************/
void FSMC_CONFIG(void)
{
	/* Bank1 NOR/SRAM control register configuration */ 
	FSMC_Bank1->BTCR[0] = 
			0x00000008 |
			0x00000010 |
			0x00001000;

	FSMC_Bank1->BTCR[0] |= 0x00000040;

	/* Bank1 NOR/SRAM timing register configuration */
	FSMC_Bank1->BTCR[1] = 
			0x02 | (0x00 << 4) | (0x05 << 8) |
			(0x00 << 16) | (0x00 << 20) | (0x00 << 24) |
			 0x10000000;
			


	FSMC_Bank1E->BWTR[0] = 0x0FFFFFFF;
	FSMC_Bank1->BTCR[0] |= 1;
}


/*******************************************************************************
 函 数 名：	STM32_CONFIG
 功能描述：	初始化stm32
 入口参数：	无
 出口参数：	无
********************************************************************************/
void STM32_CONFIG(void)
{
	RCC_CONFIG();
	GPIO_CONFIG();
	DAC1_CONFIG();
	TIM2_CONFIG();
	TIM4_CONFIG();
	FSMC_CONFIG();
//	IWDG_CONFIG();
}

/*******************************************************************************
 函 数 名：	delay_1us
 功能描述：	延时1us
 入口参数：	无
 出口参数：	无
********************************************************************************/
void delay_1us(void)
{
	u8 i = 7;
	
	while (i--);
}

/*******************************************************************************
 函 数 名：	delay_nus
 功能描述：	延时nus
 入口参数：	t 延时us数
 出口参数：	无
********************************************************************************/
void delay_nus(u16 t)
{
	while (t--)
	{
		delay_1us();
	}
}

/*******************************************************************************
 函 数 名：	delay_1ms
 功能描述：	延时1ms
 入口参数：	无
 出口参数：	无
********************************************************************************/
void delay_1ms(void)
{
	u16 i;
	
	i = 8000;
	
	while (i--);
}

/*******************************************************************************
 函 数 名：	delay_nms
 功能描述：	延时nms
 入口参数：	n:ms数
 出口参数：	无
********************************************************************************/
void delay_nms(u16 n)
{
	while (n--)
		delay_1ms();
}


