#include "stm32f10x.h"
#include "lcd.h"
#include "MM_T035.h"


void DataToWrite(u16 data) 
{
	GPIOB->ODR=((GPIOB->ODR&0x00ff)|(data&0xff00));	
	Clr_nWr;
	Set_nWr;
	GPIOB->ODR=((GPIOB->ODR&0x00ff)|(data<<8));
}

void LCD_delay(__IO u32 nCount)
{
	volatile int i;
	for(i=0;i<72000;i++)
    for(; nCount != 0; nCount--);
}
static void LCD_Rst(void)
{			
    GPIOB->BRR = 1 << 10;
//	GPIOD->BRR = 1 << 13;
    LCD_delay(100);					   
	GPIOB->BSRR = 1 << 10;
//	GPIOD->BSRR = 1 << 13;
    LCD_delay(100);	
}
 void WriteComm(u16 CMD)
{			
	*(__IO u16 *) (Bank1_LCD_C) = CMD;
}
 void WriteData(u16 tem_data)
{			
	*(__IO u16 *) (Bank1_LCD_D) = tem_data;
}

/******************************************
��������Lcdд�����
���ܣ���Lcdָ��λ��д��Ӧ�����������
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
******************************************/
void LCD_WR_REG(u16 Index,u16 CongfigTemp)
{
	*(__IO u16 *) (Bank1_LCD_C) = Index;	
	*(__IO u16 *) (Bank1_LCD_D) = CongfigTemp;
}
/************************************************
��������Lcdд��ʼ����
���ܣ�����Lcd�������� ִ��д����
************************************************/
void Lcd_WR_Start(void)
{
*(__IO u16 *) (Bank1_LCD_C) = 0x2C;
}
void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
{
	//R61408

	WriteComm(0x2a);   
	WriteData(Xstart>>8);
	WriteData(Xstart&0xff);
	WriteData(Xend>>8);
	WriteData(Xend&0xff);

	WriteComm(0x2b);   
	WriteData(Ystart>>8);
	WriteData(Ystart&0xff);
	WriteData(Yend>>8);
	WriteData(Yend&0xff);

	WriteComm(0x2c);
}
u16 GetPoint(u16 x,u16 y)
{
	 u16 a,b;
	WriteComm(0x2a);   
	WriteData(x>>8);
	WriteData(x);
	WriteData(x>>8);
	WriteData(x);

	WriteComm(0x2b);   
	WriteData(y>>8);
	WriteData(y);
	WriteData(y>>8);
	WriteData(y);

	WriteComm(0x2e);
	
	a = *(__IO u16 *) (Bank1_LCD_D);a=1;while(--a);
	a = *(__IO u16 *) (Bank1_LCD_D);
	b = *(__IO u16 *) (Bank1_LCD_D);
// 	printf("RIN=%04x\r\n",b);

	return (a&0xf800)|((a&0x00fc)<<3)|(b>>11);
}
/**********************************************
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart�� yStart������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ֹ��
          xLong Ҫѡ�����ε�x���򳤶�
          yLong  Ҫѡ�����ε�y���򳤶�
����ֵ����
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 temp;

	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<xLong*yLong; temp++)
	{
		*(__IO u16 *) (Bank1_LCD_D) = Color;
	}
}

/******************************************
��������Lcdͼ�����100*100
���ܣ���Lcdָ��λ�����ͼ��
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x3600); //Set_address_mode
// 	WriteData(0x00); //�����������½ǿ�ʼ�������ң����µ���
	BlockWrite(x,x+pic_H-1,y,y+pic_V-1);
	for (i = 0; i < pic_H*pic_V*2; i+=2)
	{
		j=pic[i];
		j=j<<8;
		j=j+pic[i+1];
		*(__IO u16 *) (Bank1_LCD_D) = j;
	}
// 	WriteComm(0x3600); //Set_address_mode
// 	WriteData(0xA0);
}


/************************************************
��������Lcdд��ʼ����
���ܣ�����Lcd�������� ִ��д����
��ڲ�������
����ֵ����
************************************************/
void Lcd_CMD_WR_Start(void)
{
	Clr_Cs;
	Clr_Rs;
	DataToWrite(0x0000);
	Clr_nWr;
	Set_nWr;
	Set_Rs;
}



/**********************************************
��������Lcdȫ����������
���ܣ���Lcd������Ϊָ����ɫ
��ڲ�����color ָ��Lcdȫ����ɫ RGB(5-6-5)
����ֵ����
***********************************************/
void Lcd_Clear(u16 Color)
{
	u32 temp;
	
	Lcd_Start();
	if((Color>>8)==(Color&0x00ff))
	{
		DataToWrite(Color);
		for (temp = 0; temp < 76800*2-1; temp++)
		{
			Clr_nWr;
			Set_nWr;
		}
	}	
	else
	{
		for (temp = 0; temp < 76800; temp++)
		{
			DataToWrite(Color);
			Clr_nWr;
			Set_nWr;
		}
	}

}
/*************************************************
��������Lcd�����㶨λ����
���ܣ�ָ��320240Һ���ϵ�һ����Ϊд���ݵ���ʼ��
��ڲ�����x ���� 0~239
          y ���� 0~319
����ֵ����
*************************************************/
void Lcd_SetCursor(u16 x,u16 y)
{ 
	//�����ת
	LCD_WR_REG(0x20,y);//ˮƽ����
	LCD_WR_REG(0x21,x);//��ֱ����  
}
/**********************************************
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart�� yStart������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ֹ��
          xLong Ҫѡ�����ε�x���򳤶�
          yLong  Ҫѡ�����ε�y���򳤶�
����ֵ����
***********************************************/


void Lcd_SetBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 x_offset,u16 y_offset)
{
	Lcd_SetCursor(xStart+x_offset,yStart+y_offset);
	LCD_WR_REG(0x0050,yStart+y_offset);//ˮƽGRAM��ʼλ��
	LCD_WR_REG(0x0051,yStart+yLong-1+y_offset);//ˮƽGRAM��ֹλ�� 

    //x,y����Ե� 
	LCD_WR_REG(0x0052,xStart+x_offset);//��ֱ GRAM��ʼλ��
	LCD_WR_REG(0x0053,xStart+xLong-1+x_offset);//��ֱGRAM��ֹλ��

	
}

void Lcd_ClearCharBox(u16 x,u16 y,u16 Color)
{
	u32 temp;
  
	Lcd_SetBox(x*8,y*16,8,16,0,0); 
	Lcd_WR_Start();
	Set_Rs;
  
	for (temp=0; temp < 128; temp++)
	{
		DataToWrite(Color); 
		Clr_nWr;
		Set_nWr;
	}
}

////////////////////////////////////////////////////////////////////////////////
void DrawPixel(u16 x, u16 y, int Color)
{
	BlockWrite(x,x,y,y);

	*(__IO u16 *) (Bank1_LCD_D) = Color;
}

//��ʼȫ��д,���õ�ַ
void Lcd_Start(void)
{  
	Lcd_SetCursor(0,239);     
	LCD_WR_REG(0x0050,0x00);//��ֱ GRAM��ʼλ��
	LCD_WR_REG(0x0051,239);//��ֱGRAM��ֹλ��
	LCD_WR_REG(0x0052,0x00);//ˮƽGRAM��ʼλ��
	LCD_WR_REG(0x0053,319);//ˮƽGRAM��ֹλ��   
	Lcd_WR_Start();
}

void display_ascii_8_8(u16 x, u16 y, char c, u16 charColor,u16 bkColor)
{
	int i, j;
	u8 cc[8] = {0x3C,0x06,0x06,0x3E,0x66,0x66,0x3E, 0x00,};
	
	BlockWrite(x,x+8-1,y,y+8-1);
	
	for (i = 0; i < 8; i++)
	{
		for (j = 8; j > 0; j--)
		{
			if (cc[i] & (1 << (j-1)))
				*(__IO u16 *) (Bank1_LCD_D) = charColor;
			else
				*(__IO u16 *) (Bank1_LCD_D) = bkColor;
		}
	}
}
	

/****************************************************************************
* ��    �ƣ�void Put32_32HZ(u16 x,u16 y,const unsigned char *c,u16 charColor,u16 bkColor,u8 flag)
* ��    �ܣ���ָ��������ʾһ������ĺ���
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ssd1289_PutChar(10,10,'a',0x0000,0xffff, 1);
****************************************************************************/
// void Put32_32HZ(u16 x,u16 y,const unsigned char *c,u16 charColor,u16 bkColor,u8 flag)
// {
//   u8 i,j,k; 
//   u8 tmp_char=0;

//   Lcd_SetBox(x,y,32,32,0,0);
//   Lcd_WR_Start();
//   Set_Rs;

//   for (i=31;i>=0;i--)//32��
//   {
//     for (j=3;j>=0;j--)//ÿ��4�ֽ�
//     {
//       tmp_char=c[4*i+j];
//       for (k=0;k<8;k++)//ÿ�ֽ�8λ
//       {
//         if (((tmp_char << k) & 0x80) == 0x80)
//         {
// 		  DataToWrite(charColor);
// 		  Clr_nWr;
// 	  	  Set_nWr;
//         }
//         else
//         {
// 		   if(flag){
// 			 DataToWrite(bkColor);
// 		     Clr_nWr;
// 	  	     Set_nWr;
// 		   }
//         }
//       }
//     }
//   }

//   Set_Cs;

// }

/****************************************************************************
* ��    �ƣ�void Put16_16HZ(u16 x,u16 y,const unsigned char *c,u16 charColor,u16 bkColor,u8 flag)
* ��    �ܣ���ָ��������ʾһ������ĺ���
* ��ڲ�����x          ������
*           y          ������
*           charColor  �ַ�����ɫ
*           bkColor    �ַ�������ɫ
* ���ڲ�������
* ˵    ������ʾ��Χ�޶�Ϊ����ʾ��ascii��
* ���÷�����ssd1289_PutChar(10,10,'a',0x0000,0xffff, 1);
****************************************************************************/
void Put16_16HZ(u16 x,u16 y,const unsigned char *c,u16 charColor,u16 bkColor,u8 flag)
{
  char i,j,k;
  
  u8 tmp_char=0;

  Lcd_SetBox(x,y,16,16,0,0);
  Lcd_SetCursor(x,y+15);
  Lcd_WR_Start();
  Set_Rs;

  for (i=0;i<16;i++)//16��
  {
    for (j=0;j<2;j++)//ÿ��2�ֽ�
    {
      tmp_char=c[2*(15-i)+j];
      for (k=0;k<8;k++)//ÿ�ֽ�8λ
      {
        if (((tmp_char << k) & 0x80) == 0x80)
        {
		  DataToWrite(charColor);
		  Clr_nWr;
		  Set_nWr;
        }
        else
        {
			 DataToWrite(bkColor);
		     Clr_nWr;
		     Set_nWr;
        }
      }
    }
  }
  Set_Cs;

}


void Put8_16Char(u16 x,u16 y,const unsigned char *ascii_8x16,u8 ch,u16 charColor,u16 bkColor,u8 flag)
{
  char i=0,j=0;
  u8 tmp_char=0;

  Lcd_SetBox(x,y,8,16,0,0);
  Lcd_SetCursor(x,y+15);
  Lcd_WR_Start();
  Set_Rs;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((ch-0x20)*16)+(15-i)];
    for (j=0;j<8;j++)
    {
        if(((tmp_char<<j) & 0x80) == 0x80)
        {
		  DataToWrite(charColor);
		  Clr_nWr;
		  Set_nWr;
        }
        else
        {
		   	 DataToWrite(bkColor);  //0x7BEF
		     Clr_nWr;
		     Set_nWr;
        }
    }
  }
  Set_Cs;
}



void Put16x32Num(u16 x,u16 y,const unsigned char *num_16x32,u8 ch,u16 charColor,u16 bkColor,u8 flag)
{
  char i=0,j=0,k=0;
  u8 tmp_char=0;

  Lcd_SetBox(x,y,16,32,0,0);
  Lcd_SetCursor(x,y+31);
  Lcd_WR_Start();
  for (i=0;i<32;i++)
  {
    for (k=0;k<2;k++)
	{
      tmp_char=num_16x32[ch*32*2+(31-i)*2+k];
      for (j=0;j<8;j++)
      {
        if(((tmp_char<<j) & 0x80) == 0x80)
        {
		  DataToWrite(charColor);
		  Clr_nWr;
		  Set_nWr;
        }
        else
        {
		   	 DataToWrite(bkColor);  //0x7BEF
		     Clr_nWr;
		     Set_nWr;

        }
      }
	}
  }
}

void LCD_Scan_Set(char i)
{
	
  if(i==0)	{LCD_WR_REG(0x0003,0x1038);Lcd_SetCursor(0,239);}//
	if(i==1)	{LCD_WR_REG(0x0003,0x1028);Lcd_SetCursor(0,239);}//
  if(i==2)	{LCD_WR_REG(0x0003,0x1008);Lcd_SetCursor(319,0);}//
	if(i==3)	{LCD_WR_REG(0x0001,0x0100);return;}//
	LCD_WR_REG(0x0050,0x00);//��ֱGRAM��ʼλ��
	LCD_WR_REG(0x0051,239);//��ֱGRAM��ֹλ��
	LCD_WR_REG(0x0052,0x00);//ˮƽGRAM��ʼλ��
	LCD_WR_REG(0x0053,319);//ˮƽGRAM��ֹλ��   
	Lcd_WR_Start();
}
void lcd_init(void)
{	
	LCD_delay(20);
	LCD_Rst();
	
	Lcd_Light_ON;
//************* Start Initial Sequence **********// 	
	WriteComm(0xFF); // EXTC Command Set enable register
	WriteData(0xFF);
	WriteData(0x98);
	WriteData(0x06);
	WriteComm(0xBA); // SPI Interface Setting
	WriteData(0x60);
	WriteComm(0xBC); // GIP 1
	WriteData(0x03);
	WriteData(0x0E);
	WriteData(0x03);
	WriteData(0x63);
	WriteData(0x01);
	WriteData(0x01);
	WriteData(0x1B);
	WriteData(0x12);
	WriteData(0x6F);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x01);
	WriteData(0x01);
	WriteData(0x03);
	WriteData(0x02);
	WriteData(0xFF);
	
	WriteData(0xF2);
	WriteData(0x01);
	WriteData(0x00);
	WriteData(0xC0);
	WriteComm(0xBD); // GIP 2
	WriteData(0x02);
	WriteData(0x13);
	WriteData(0x45);
	WriteData(0x67);
	WriteData(0x45);
	WriteData(0x67);
	WriteData(0x01);
	WriteData(0x23);
	WriteComm(0xBE); // GIP 3
	WriteData(0x01);
	WriteData(0x22);
	WriteData(0x22);
	WriteData(0xDC);
	WriteData(0xBA);
	WriteData(0x67);
	WriteData(0x22);
	WriteData(0x22);
	WriteData(0x22);
	WriteComm(0xC7); // VCOM Control
	WriteData(0x66);
	WriteComm(0xED);
	WriteData(0x7F);
	WriteData(0x0F);
	WriteData(0x00);
	WriteComm(0xC0); // Power Control 1
	WriteData(0x03);
	WriteData(0x0B);
	WriteData(0x00);
	
	WriteComm(0XF5); // LVGL
	WriteData(0x20);
	WriteData(0x43);
	WriteData(0x00);
	WriteComm(0xEE); // LVGL
	WriteData(0x0A);
	WriteData(0x1B);
	WriteData(0x5F);
	WriteData(0x40);
	WriteData(0x28);
	WriteData(0x38);
	WriteData(0x02);
	WriteData(0x2B);
	WriteData(0x50);
	WriteData(0x00);
	WriteData(0x80);
	WriteComm(0xFC); // LVGL
	WriteData(0x08);
	WriteComm(0xDF);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x20);
	WriteComm(0xF3);
	WriteData(0x74);
	WriteComm(0xB4); // Display Inversion Control
	WriteData(0x02);
	WriteData(0x02);
	WriteData(0x02);
	WriteComm(0xF7); // 480x800
	WriteData(0x81);
	
	WriteComm(0xB1); // Frame Rate Control
	WriteData(0x00);
	WriteData(0x13);
	WriteData(0x13);
	WriteComm(0xF2); //Panel Timing Control
	WriteData(0x41);
	WriteData(0x04);
	WriteData(0x41);
	WriteData(0x28);
	WriteComm(0xC1); // Power Control 2
	WriteData(0x17);
	WriteData(0x78);
	WriteData(0x7B);
	WriteData(0x20);
	WriteComm(0xE0);
	WriteData(0x00); //P1
	WriteData(0x02); //P2
	WriteData(0x0C); //P3
	WriteData(0x0F); //P4
	WriteData(0x11); //P5
	WriteData(0x1C); //P6
	WriteData(0xC8); //P7
	WriteData(0x07); //P8
	WriteData(0x03); //P9
	WriteData(0x08); //P10
	WriteData(0x03); //P11
	WriteData(0x0D); //P12
	WriteData(0x0C); //P13
	WriteData(0x31); //P14
	WriteData(0x2C); //P15
	WriteData(0x00); //P16
	WriteComm(0xE1);
	WriteData(0x00); //P1
	WriteData(0x02); //P2
	WriteData(0x08); //P3
	WriteData(0x0E); //P4
	WriteData(0x12); //P5
	WriteData(0x17); //P6
	WriteData(0x7C); //P7
	WriteData(0x0A); //P8
	WriteData(0x03); //P9
	WriteData(0x09); //P10
	WriteData(0x06); //P11
	WriteData(0x0C); //P12
	WriteData(0x0C); //P13
	WriteData(0x2E); //P14
	WriteData(0x2A); //P15
	WriteData(0x00); //P16
	WriteComm(0x35); //Tearing Effect ON
	WriteData(0x00);
	WriteComm(0x36); 
	WriteData(0x60);
	WriteComm(0x3A); 
	WriteData(0x55);
	WriteComm(0x11); //Exit Sleep
	LCD_delay(120);
	WriteComm(0x29); // Display On

	Lcd_Light_ON;
	
	Lcd_ColorBox(0,0,800,480,YELLOW);
	Lcd_ColorBox(0,0,854,480,BLACK);
	
//	LCD_Fill_Pic(0,  0, 320, 480, gImage_MM_T035);
//	LCD_Fill_Pic(400,0, 320, 480, gImage_MM_T035);
	LCD_Fill_Pic(300,0, 320, 480, gImage_MM_T035);	
//	LCD_Fill_Pic(80,160,320,480, gImage_MM_T035);
//	BlockWrite(0,0,799,479);

  display_ascii_8_8(200, 100, 'a', RED, YELLOW);
  display_ascii_8_8(200, 120, 'a', WHITE, BLACK);
}

/*---------------------------------------------------------------------------
���ܣ�	��ֱ����
��ڲ�����line_num  ��������
---------------------------------------------------------------------------*/
void lcd_scrol_screen(unsigned int line_num)
{
	 LCD_WR_REG(0x006a, line_num);// set scrolling line	
	 LCD_WR_REG(0x0061, 0x0003);//enable scrol

//	 LCD_WR_REG(0x006a, line_num);// set scrolling line	
//	 LCD_WR_REG(0x0061, 0x0001);////disable scrol
}

/*---------------------------------------------------------------------------
���ܣ�	��ֹ��ֱ����
---------------------------------------------------------------------------*/
void lcd_scrol_disable(void)
{
	 LCD_WR_REG(0x006a, 0x0000);// set scrolling line	
	 LCD_WR_REG(0x0061, 0x0001);//disable scrol
}

