/*
 * p1.c
 *
 *  Created on: 3 May 2021
 *      Author: Rana Mohamed
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char ticks=0;
unsigned char sec1 =0;
unsigned char sec2 =0;
unsigned char min1 =0;
unsigned char min2 =0;
unsigned char hour1 =0;
unsigned char hour2 =0;



ISR(TIMER1_COMPA_vect){

ticks++;

if(ticks == 1)
{
   	sec1++;
	TCNT1=0;
	ticks=0;
}


}


ISR(INT0_vect){
	//rest the stop watch
 	 sec1 =0;
	 sec2 =0;
	 min1 =0;
	 min2 =0;
	 hour1 =0;
	 hour2 =0;

	TCNT1=0;


}


ISR(INT1_vect){
	//pause the stop watch
	TCCR1B =  (1<<WGM12) ;
}


ISR(INT2_vect){
	//resume the stop watch
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10); ;
}


void Int0_set(void){
	//falling edge
	GICR |=(1<<INT0);
	MCUCR|=(1<<ISC01);
}


void Int1_set(void){
	//rising edge
	GICR |=(1<<INT1);
	MCUCR|=(1<<ISC10)|(1<<ISC11);
}


void Int2_set(void){
	//Falling edge so MCUCSR BIT ISC2 is initially zero;
	GICR |=(1<<INT2);

}


void Timer1_set(void){

TCNT1=0;
OCR1A= 1000;
TIMSK= (1<<OCIE1A);
TCCR1A = (1<<FOC1A)|(1<<FOC1B);
TCCR1B= (1<<WGM12) | (1<<CS12) | (1<<CS10);
TIMSK= (1<<OCIE1A);

}



int main(){
	SREG |=(1<<7);
	// activate the internal pull up of INT0, INT2
	DDRD &=(1<<PD2);
	PORTD |=(1<<PD2);
	DDRB &=(1<<PB2);
	PORTB |=(1<<PB2);
	//set the portC as output for the seven segment
	DDRA |= 0x3F;
	PORTA = 0x3F;
	DDRC |=0x0F;
	PORTC=0;
	Int0_set();
	Int1_set();
	Int2_set();
    Timer1_set();


    while(1)
    {

    	if((sec1 > 9) && (sec2 < 6 ))
    	   	   {
    	   	       sec1=0;
    	   	       sec2++;
    	   	    }
    	   	if ((sec2==6))
    	   	{
    	       	 sec1=0;
    	     	 sec2=0;
    	         min1++;
    	   	 }
    	    if((min1 > 9) && (min2 < 6))
    	    {
    	    	min1=0;
    	    	min2++;

    	    }
    	    if((min2==6))
    	    {
    	    	min1=0;
    	    	min2=0;
    	    	hour1++;
    	    }
    	    if((hour1>9) && ( hour2 <9))
    	    {
    	    	hour1=0;
    	    	hour2++;
    	    }
    	    if(hour2==9){
    	    	hour1=0;
    	    	hour2=0;
    	    }

       PORTA= (1<<PA0);
       PORTC= sec1;

       _delay_us(0);

        PORTA = (1<<PA1);
        PORTC = sec2;
       _delay_us(0);


        PORTA= (1<<PA2);
        PORTC = min1;
        _delay_us(0);


        PORTA= (1<<PA3);
        PORTC = min2;
        _delay_us(0);


        PORTA= (1<<PA4);
        PORTC = hour1;
        _delay_us(0);

        PORTA= (1<<PA5);
        PORTC = hour2;
        _delay_us(0);


    }



}

