#include "stdint.h"
#include "C:\Keil\EE319Kware\inc\tm4c123gh6pm.h"
 /* bit addressing of port A*/
#define pA2            (*((volatile uint32_t *)0x40004010))
#define pA3            (*((volatile uint32_t *)0x40004020))
#define pA4            (*((volatile uint32_t *)0x40004040))
#define pA5            (*((volatile uint32_t *)0x40004080))
#define pA6            (*((volatile uint32_t *)0x40004100))
#define pa7            (*((volatile uint32_t *)0x40004200))
	/* bit addressing of port B*/
#define pB0            (*((volatile uint32_t *)0x40005004))
#define pB1            (*((volatile uint32_t *)0x40005008))
#define pB2            (*((volatile uint32_t *)0x40005010))
#define pB3            (*((volatile uint32_t *)0x40005020))
#define pB4            (*((volatile uint32_t *)0x40005040))
#define pB5            (*((volatile uint32_t *)0x40005080))
#define pB6            (*((volatile uint32_t *)0x40005100))
#define pB7            (*((volatile uint32_t *)0x40005200))
  /* bit addressing of port D*/
#define pD0            (*((volatile uint32_t *)0x40007004))
#define pD1            (*((volatile uint32_t *)0x40007008))
#define pD2            (*((volatile uint32_t *)0x40007010))
#define pD3            (*((volatile uint32_t *)0x40007020))
#define pD6            (*((volatile uint32_t *)0x40007100))
#define pD7            (*((volatile uint32_t *)0x40007200))
	/* bit addressing of port E*/
#define pE0            (*((volatile uint32_t *)0x40024004))
#define pE1            (*((volatile uint32_t *)0x40024008))
#define pE2            (*((volatile uint32_t *)0x40024010))
#define pE3            (*((volatile uint32_t *)0x40024020))
#define pE4            (*((volatile uint32_t *)0x40024040))
#define pE5            (*((volatile uint32_t *)0x40024080))

void SystemInit(){}
void init_portB()
{
  uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x02 ; //CLK
	delay = 1; //dummy var
	GPIO_PORTB_AMSEL_R =0;
	GPIO_PORTB_AFSEL_R = 0;
	GPIO_PORTB_PCTL_R = 0;
	GPIO_PORTB_DIR_R = 0xF3;
	GPIO_PORTB_DEN_R = 0xF3;
}

void init_portD()
{
  uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x08 ; //CLK
	delay = 1; //dummy var
	GPIO_PORTD_AMSEL_R =0;
	GPIO_PORTD_AFSEL_R = 0;
	GPIO_PORTD_PCTL_R = 0;
	GPIO_PORTD_DIR_R = 0x0E;
	GPIO_PORTD_DEN_R = 0x1F;
}

void sys_init_timer()
{
	NVIC_ST_CTRL_R =0; //CONTROL REG 
	NVIC_ST_RELOAD_R = 16000; // 1msec
	NVIC_ST_CURRENT_R =0; //TO CLEAR THE FLAG 
	NVIC_ST_CTRL_R =5; //101 ENABLE AND CLK SRC AND 0 FOR INTERRUPT
}


void delay (uint32_t counts) //max delay 1 sec 0xffffff
{
	NVIC_ST_CTRL_R =0; //CONTROL REG 
	NVIC_ST_RELOAD_R =counts ; // 1msec
	NVIC_ST_CURRENT_R =0; //TO CLEAR THE FLAG
	NVIC_ST_CTRL_R =5;
	while((NVIC_ST_CTRL_R&0x00010000)==0)
	{}	
}

int main ()
{
	
	uint32_t counter =0;  //counter for calculate the time  
	float Distance = 0;
	uint32_t units,tenth,hundreds;
  init_portB();
	init_portD();
	while (1)
	{	 		
		GPIO_PORTB_DATA_R |=0x04;  // enable the trigger pin
		delay (0x10);             //delay 1 microsecond
		GPIO_PORTB_DATA_R &=0;   // disable the trigger pin
		sys_init_timer();
		/* counter still counts until echo signal comes */
		while(GPIO_PORTB_DATA_R!=0x08)
		{
		  if(NVIC_ST_CTRL_R&0x00010000)
		   {
		   	sys_init_timer();
		  	counter++;	   //  counts every 1 ms
		   }
	  }
		Distance = 34.4*(counter/2.0);	// Distance in centimeters	
		counter=0;
		/* display  */
		units = (int)Distance % 10;
		tenth = ((int)Distance/10) % 10;
		hundreds = (((int)Distance/10)/10) % 10;
	  //D0 D1 D2 D3 pins for units digit 
		 switch (units)
		 {
			 case 0:
				 pD3=0; pD2=0; pD1=0; pD0=0;
         break;
			 case 1:
				 pD3=0; pD2=0; pD1=0; pD0=1;
         break;
			 case 2:
				 pD3=0; pD2=0; pD1=1; pD0=0;
         break;
			 case 3:
				 pD3=0; pD2=0; pD1=1; pD0=1;
         break;
			 case 4:
				 pD3=0; pD2=1; pD1=0; pD0=0;
         break;
			 case 5:
				 pD3=0; pD2=1; pD1=0; pD0=1;
         break;
			 case 6:
				 pD3=0; pD2=1; pD1=1; pD0=0;
         break;
			 case 7:
				 pD3=0; pD2=1; pD1=1; pD0=1;
         break;
			 case 8:
				 pD3=1; pD2=0; pD1=0; pD0=0;
         break;
			 case 9:
				 pD3=1; pD2=0; pD1=0; pD0=1;
         break;
			 default:
				 pD3=0; pD2=0; pD1=0; pD0=0;
         
		 }
		 
		 //D6 D7 B0 B1 pins for tenth digit 
		 switch (tenth)
		 {
			 case 0:
				 pB1=0; pB0=0; pD7=0; pD6=0;
         break;
			 case 1:
				 pB1=0; pB0=0; pD7=0; pD6=1;
         break;
			 case 2:
				 pB1=0; pB0=0; pD7=1; pD6=0;
         break;
			 case 3:
				 pB1=0; pB0=0; pD7=1; pD6=1;
         break;
			 case 4:
				 pB1=0; pB0=1; pD7=0; pD6=0;
         break;
			 case 5:
				 pB1=0; pB0=1; pD7=0; pD6=1;
         break;
			 case 6:
				 pB1=0; pB0=1; pD7=1; pD6=0;
         break;
			 case 7:
				 pB1=0; pB0=1; pD7=1; pD6=1;
         break;
			 case 8:
				 pB1=1; pB0=0; pD7=0; pD6=0;
         break;
			 case 9:
				 pB1=1; pB0=0; pD7=0; pD6=1;
         break;
			 default:
				 pB1=0; pB0=0; pD7=0; pD6=0;
         
		 }
		 
		 //B5 B4 B3 B2 pins for hundreds digit 
		 switch (hundreds)
		 {
			 case 0:
				 pB5=0; pB4=0; pB3=0; pB2=0;
         break;
			 case 1:
				 pB5=0; pB4=0; pB3=0; pB2=1;
         break;
			 case 2:
				 pB5=0; pB4=0; pB3=1; pB2=0;
         break;
			 case 3:
				 pB5=0; pB4=0; pB3=1; pB2=1;
         break;
			 case 4:
				 pB5=0; pB4=1; pB3=0; pB2=0;
         break;
			 case 5:
				 pB5=0; pB4=1; pB3=0; pB2=1;
         break;
			 case 6:
				 pB5=0; pB4=1; pB3=1; pB2=0;
         break;
			 case 7:
				 pB5=0; pB4=1; pB3=1; pB2=1;
         break;
			 case 8:
				 pB5=1; pB4=0; pB3=0; pB2=0;
         break;
			 case 9:
				 pB5=1; pB4=0; pB3=0; pB2=1;
         break;
			 default:
				 pB5=0; pB4=0; pB3=0; pB2=0;
         
		 }		
	}	
}
