#include<iostream>
using namespace std;
//--------------APB2使能时钟寄存器------------------------
#define RCC_AP2ENR	*((unsigned volatile int*)0x40021018)
	//----------------GPIOA配置寄存器 ------------------------
#define GPIOA_CRL	*((unsigned volatile int*)0x40010800)
#define	GPIOA_ORD	*((unsigned volatile int*)0x4001080C)
//----------------GPIOB配置寄存器 ------------------------
#define GPIOB_CRH	*((unsigned volatile int*)0x40010C04)
#define	GPIOB_ORD	*((unsigned volatile int*)0x40010C0C)
//----------------GPIOC配置寄存器 ------------------------
#define GPIOC_CRH	*((unsigned volatile int*)0x40011004)
#define	GPIOC_ORD	*((unsigned volatile int*)0x4001100C)
//-------------------简单的延时函数-----------------------
void delay(unsigned long xx)
{
	while(xx--);	//用自减的方式粗略延时
}
//------------------------主函数--------------------------
int main()
{
	cout<<RCC_AP2ENR<<endl;
}

