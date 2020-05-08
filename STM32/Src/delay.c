
#include "delay.h"


void delay_us(uint32_t us){ 

	for(int i=0; i<us*6; i++)
  {
	}
}

void delay_ms(uint32_t ms){ 

	for(int i=0; i<ms; i++)
  {
		delay_us(1000);
	}
}





























