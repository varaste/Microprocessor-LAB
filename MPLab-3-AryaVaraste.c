#include <avr\io.h>
#include <avr\interrupt.h>
#include <util\delay.h>


int main(){
	
	DDRA |= (1<<PA0);
	DDRA |= (1<<PA1);
	DDRA |= (1<<PA2);	

	while(1){
		_delay_ms(750);
		PORTA = 0b00000001;
		_delay_ms(750);
		PORTA = 0b00000010;
		_delay_ms(750);
		PORTA = 0b00000100;
		_delay_ms(750);
	}
	return 0;
}




#include<avr\io.h>
#include<avr\interrupt.h>
#include<util\delay.h>


int main(){
	
	DDRA |= (1 << PAO);	

	while(1){
		PORTA |= 1;
		_delay_ms(750);
		PORTA &= ~(1);
		_delay_ms(750);
	}

	return 0;
}
