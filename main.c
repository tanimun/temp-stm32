#include "stm32f10x.h"                  // Device header
#include<stdio.h>

#define RS 0x0020;
#define EN 0x0080;

void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_number(unsigned int number);
void delayMs(int Delay);

int main(void)
{
	
	char a[] = { "TEMP:" };
	char b[] = { "STATUS:" };
	char c[] = { "NOT NORMAL:" };
	char d[] = { "NORMAL:" };
	char e[] = { "DANGER:" };

	int analogValue, voltageout;
	int temp;
	
	
	RCC-> APB2ENR |= 0x020C; // enable GPIOA, GPIOB, GPIOC
	
	GPIOB -> CRL = 0x33333333;
	
	GPIOA -> CRL = 0x30304A00;

	LCD_command(0x38);
	delayMs(10);
	LCD_command(0x38);
	delayMs(1);
	LCD_command(0x38);
	delayMs(1);
	LCD_command(0x0E);
	delayMs(1);
	LCD_command(0x01);
	delayMs(1);
	LCD_command(0x06);
	delayMs(1);
	LCD_command(0x80);
	delayMs(1);
	
	for(int i =0; a[i]!=0; i++)
	
	{
		LCD_data(a[i]);
	
	}
	LCD_command(0xC0);
	delayMs(1);
	for(int i = 0; b[i]!=0; i++)
	{
		LCD_data(b[i]);
	
	}
		
	RCC-> CFGR |= 0x8000;
	ADC1 -> SMPR2 |= 0x0038;
	ADC1 -> SQR3 |= 0x0001;
	ADC1->CR2 |= 0x0003;
	delayMs(10);
	ADC1 ->CR2 |= 0x0003;
	delayMs(5);
	ADC1->CR2 |= 0x0007;
	delayMs(10);
	
	while(1)
	{
		
	ADC1->CR2 |= 0x00400000;
	while(!(ADC1-> SR & 2)) {}
	analogValue = ADC1-> DR;
	voltageout = (analogValue*5000)/4096;
	temp = voltageout/10;
		//temp = 3.3*(analogValue*3.2/100);
	
	LCD_command(0x87);
	delayMs(1);
	LCD_data(0x30+(temp%100)/10);
	delayMs(1);
	LCD_data(0x30+(temp%10));
	delayMs(1);
	LCD_command(0xC7);
	delayMs(1);
	
	if(temp<=35)	
	{
		for(int i = 0; c[1]!=0; i++)
		{
			LCD_data(c[i]);
			
		}
	}
	else if(temp>35&&temp<38){
	
	for (int i =0;d[i]!=0; i++)
		{
		
		LCD_data(d[i]);
	}
}
	else
	{
		for(int i =0; e[i]!=0; i++)
		{
		LCD_data(e[i]);
		
			}
		}		
	}
}	
void LCD_command (unsigned char command)
{
	GPIOA -> ODR &= ~RS;
	GPIOB -> ODR = command;
	GPIOA -> ODR |=EN;
	delayMs(1);
	GPIOA -> ODR &= ~EN;
	delayMs(1);
}

void LCD_data (char data)
{
	GPIOA -> ODR |= RS;
	GPIOB -> ODR = data;
	GPIOA -> ODR |=EN;
	delayMs(1);
	GPIOA->ODR &= ~EN;
	delayMs(1);
}

void delayMs(int delay)
{
	for(;delay>0;delay--)
	{
	
		for(int i =0; i<5000; i++)
		{}	
	}
}
