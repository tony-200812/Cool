#include "stm32f10x.h"


/*******************************************************************************
 �� �� ����	RCC_CONFIG
 ����������	ʹ������ʱ��
 ��ڲ�����	��
 ���ڲ�����	��
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
 �� �� ����	GPIO_CONFIG
 ����������	GPIO ����
 ��ڲ�����	��
 ���ڲ�����	��
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
 �� �� ����	DAC1_CONFIG
 ����������	DAC1 ����
 ��ڲ�����	��
 ���ڲ�����	��
********************************************************************************/
void DAC1_CONFIG(void)
{
	DAC->CR  = 0x83C;
	DAC->CR |= 1;			//DAC1ʹ��
	
	DAC->DHR12R1  = 0;		
	DAC->SWTRIGR |= 0x01;	//�������ת��
}

/*******************************************************************************
 �� �� ����	TIM4_Config
 ����������	TIM4 ���� ����DAC1ת��
 ��ڲ�����	��
 ���ڲ�����	��
********************************************************************************/
void TIM4_CONFIG(void)
{
 	TIM4->ARR   = 125;
	TIM4->PSC   = 72;  					//Ԥ��Ƶ��
	
	TIM4->DIER |= 1 << 0;   			//��������ж�				
	TIM4->DIER |= 1 << 6;   			//�������ж�
		  							
	TIM4->CR1 |= 0x01;    				//ʹ�ܶ�ʱ��4
	
  	NVIC->IP[TIM4_IRQn] = 0x00;        //�ж����ȼ�
    NVIC->ISER[TIM4_IRQn >> 0x05] = (u32)0x01 << (TIM4_IRQn & (u8)0x1F);//	
}

/*******************************************************************************
 �� �� ����	USART2_CONFIG
 ����������	USART2�������� (����,19200bps 1λ��ʼλ��1λ����λ��9λ����λ)
 ��ڲ�����	��
 ���ڲ�����	��
********************************************************************************/
void USART2_CONFIG(void)
{
	USART2->CR1 = 0;
	USART2->CR1 = 1 << 12 | 1 << 6 | 1 << 5 | 1 << 3 | 1 << 2;	
	USART2->BRR = 0x0753;  		
	USART2->CR2 = 0 << 14 | 0 << 12 | 0 << 11;
	USART2->CR3 = 0;
	USART2->CR1 |= 1 << 13;	
	
	NVIC->IP[USART2_IRQn] = 0x20;		//�ж����ȼ�
	NVIC->ISER[USART2_IRQn >> 0x05] = (u32)0x01 << (USART2_IRQn & (u8)0x1F);//���ж�	
}

/*******************************************************************************
 �� �� ����	USART3_CONFIG
 ����������	USART3�������� (����)
 ��ڲ�����	��
 ���ڲ�����	��
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
 �� �� ����	SPI1_CONFIG
 ����������	SPI1 ���� (FLASH)
 ��ڲ�����	��
 ���ڲ�����	��
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
 �� �� ����	SPI2_CONFIG
 ����������	SPI2 ���� (fm1702)
 ��ڲ�����	��
 ���ڲ�����	��
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
 �� �� ����	EXTI1_CONFIG
 ����������	�ⲿ�ж�����(�����ж�)
 ��ڲ�����	��
 ���ڲ�����	��
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
 �� �� ����	EXTI7_CONFIG
 ����������	�ⲿ�ж�����(fm1702�ж�)
 ��ڲ�����	��
 ���ڲ�����	��
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
 �� �� ����	TIM2_CONFIG
 ����������	TIM2 ��ʼ�� 20ms�ж�һ�� �ж����ȼ� 3 
 ��ڲ�����	��
 ���ڲ�����	��
********************************************************************************/
void TIM2_CONFIG(void)
{	
	u32 temp;
	
	temp = SystemCoreClock / 4 / 1000;	
	
	TIM2->CR1   = 0;
	TIM2->PSC   = 3;	  					//Ԥ��Ƶֵ
	TIM2->RCR   = 0;    					//�ظ�����ֵ,�´�������Ч
	TIM2->EGR   = 1 << 0;					//���³�ʼ���Ĵ���
	TIM2->CCER &= ~(1 << 0); 				//�ߵ�ƽ��Ч������ͨ����
	TIM2->CR2   = 0;     					//û��
	TIM2->CR1  |= 0x0080;  					//ֵװ�뻺����
	TIM2->CCMR1 = (1 << 11) | (7 << 12);  	//����ͨ��4
	
	TIM2->ARR   = temp;
	TIM2->CCR2  = temp;

	TIM2->CCER  = (1 << 4) | (1 << 5);  	//ʹ�����
	TIM2->CR1  |= 1 << 0;
}

/*******************************************************************************
 �� �� ����	IWDG_CONFIG
 ����������	�������Ź���ʼ��
 ��ڲ�����	��
 ���ڲ�����	��
********************************************************************************/
void IWDG_CONFIG(void)
{
	IWDG->KR  = 0x5555; 
	IWDG->PR  = 0x04;	//64��Ƶ
	IWDG->RLR = 0x780;	//3s ���Ź��ж�
	IWDG->KR  = 0xCCCC;	//�������Ź�
	IWDG->KR  = 0xAAAA; //ι��
}


/*******************************************************************************
 �� �� ����	FSMC_Config
 ����������	LCD���߽ӿ�����
 ��ڲ�����	��
 ���ڲ�����	��
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
 �� �� ����	STM32_CONFIG
 ����������	��ʼ��stm32
 ��ڲ�����	��
 ���ڲ�����	��
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
 �� �� ����	delay_1us
 ����������	��ʱ1us
 ��ڲ�����	��
 ���ڲ�����	��
********************************************************************************/
void delay_1us(void)
{
	u8 i = 7;
	
	while (i--);
}

/*******************************************************************************
 �� �� ����	delay_nus
 ����������	��ʱnus
 ��ڲ�����	t ��ʱus��
 ���ڲ�����	��
********************************************************************************/
void delay_nus(u16 t)
{
	while (t--)
	{
		delay_1us();
	}
}

/*******************************************************************************
 �� �� ����	delay_1ms
 ����������	��ʱ1ms
 ��ڲ�����	��
 ���ڲ�����	��
********************************************************************************/
void delay_1ms(void)
{
	u16 i;
	
	i = 8000;
	
	while (i--);
}

/*******************************************************************************
 �� �� ����	delay_nms
 ����������	��ʱnms
 ��ڲ�����	n:ms��
 ���ڲ�����	��
********************************************************************************/
void delay_nms(u16 n)
{
	while (n--)
		delay_1ms();
}


