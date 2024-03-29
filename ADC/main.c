#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "stdio.h"

void enable_pinini_degis(void);
void lcd_komut_yolla(unsigned char komut);
void lcd_basla(void);
void lcdHucreyeGit(unsigned int satir,unsigned int sutun);
void lcd_veri_yaz( char  veri);
void pot_deger(char *deger);
char sAdcValue[5];

void adc_init(void);

uint16_t ADC_deger;

//a0 ve a2 output a1 input b4 b5 b6 b7 output.
int main()
	
{
	
	delay_init();
	RCC->APB2ENR |=1<<2;//A portu aktif edildi.
	RCC->APB2ENR |=1<<3;//B portu aktif edildi.
	
	GPIOA ->CRL &=0x00;//A portu temizlendi.
	GPIOA ->CRL =0x00000202;//A0 ve A2 Output.a1 input.
	
	GPIOB ->CRL &=0x00;//B portu temizlendi.
	GPIOB ->CRL  =0x22220000;//B7,B6,B5,B4 Output.
	
	GPIOB ->ODR =0x00;//B portu 0 landi.
	delay_ms(50);
	
	GPIOA ->BRR = 1<<0;//A0 reset.
	GPIOA ->BRR =1<<2; //A2 reset.
	

	for(int i=0;i<2;i++)
{

   lcd_komut_yolla(0x03);
   delay_ms(5);//4,1ms gerek
   
}
lcd_komut_yolla(0x03);
delay_ms(10);//150 mikrosaniye gerek.
lcd_basla();


lcdHucreyeGit(1,1);
delay_ms(10);//37 us diyor.
	
	adc_init();
	
	while(1)
	{
		lcd_komut_yolla(0x01);
		ADC1->CR2 |=1<<0;
		//ADC1->CR2  |=1<<22;
		while (!(ADC1->SR & (1<<1)));
		ADC_deger = ADC1->DR;
		sprintf(sAdcValue, "%d   ", ADC_deger);
		pot_deger(sAdcValue);
		ADC1->SR &= ~(1<<1);
		delay_ms(50);
		
	}
	
}

void adc_init()
{
	
	RCC->APB2ENR |=1<<9;//ADC1 ENABLE.
  ADC1->CR2   &=~(1<<1);//CONT=0
	ADC1 ->CR2   &=~(1<<11);//SAGA HIZALAMA.
	ADC1->CR2   &=~(1<<20); //EXTERNAL TRIG YOK.
	
	//RCC-> CFGR &= ~(3<<14); 
	//RCC-> CFGR |= (2<<14);
	
	
	//RCC->CFGR |=1<<15;
	//RCC->CFGR  &=1<<14;
	
	
	
	ADC1->SMPR2 |=1<<3;
	ADC1->SMPR2 &=~(1<<4);
	ADC1->SMPR2  |=1<<5;
	
	ADC1->SQR1 = 0x00000000; // only one conversion
	ADC1 ->SQR3 =0x01; // PA1 I�IN
	
	ADC1->CR2 |=1<<22;//SWSTART
	ADC1->CR2 |=1<<0;//ADC ON.
	
	
	//ADC1->CR1 |= (1<<5); // EOC interrupt
   //NIVC->ISER[0] |= (1<<18); //Interrupt no 18
	
}
void enable_pinini_degis(void)
{
	GPIOA ->BRR =1<<2;
	delay_ms(1);//1 mikro sn.
	GPIOA ->BSRR =1<<2;
	delay_ms(1);//1 mikro sn.
	GPIOA ->BRR =1<<2;
	delay_ms(1);//100 mikro sn.
}

void lcd_komut_yolla(unsigned char komut)
{
	
	GPIOB ->ODR =komut;
	GPIOA ->BRR =1<<0;//A0 reset.
	
	enable_pinini_degis();
	
	komut=komut<<4;

  GPIOB ->ODR=komut;
  GPIOA ->BRR =1<<0;//A0 reset.
  enable_pinini_degis();
	
}
void lcd_basla()
{
  lcd_komut_yolla(0x02);
  delay_ms(2);
 // lcd_komut_yolla(0x28);
	lcd_komut_yolla(0b0000000000101000);
	
  delay_ms(1);
  lcd_komut_yolla(0x0C);
  delay_ms(1);
  lcd_komut_yolla(0x01);
  delay_ms(1);
  lcd_komut_yolla(0x06);
  delay_ms(1);

}
void lcdHucreyeGit(unsigned int satir,unsigned int sutun)
{
  if(satir==1)
  {
    sutun--;
    lcd_komut_yolla(0x80+sutun);
    
    
  }
  else if(satir==2)
  {
    sutun--;
    lcd_komut_yolla(0xC0+sutun);
    
  }
}

void lcd_veri_yaz( char	veri)
{
  GPIOA->BSRR =1<<0;//A0 set.
  GPIOB ->ODR =veri;
  enable_pinini_degis();
  
  veri=veri<<4;

  GPIOA->BSRR =1<<0;//A0 set.
  GPIOB ->ODR =veri;
  enable_pinini_degis();
}

void pot_deger(char *deger)
{
	uint8_t i=0;
	while(deger[i] !='\0')
	{
		lcd_veri_yaz(deger[i]);
		i++;
		
	}
}



