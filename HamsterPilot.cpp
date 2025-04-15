#include <avr/io.h>
#include <avr/interrupt.h>

class motorSystem{
	private:
	uint8_t status = 0;
	public:
	
	motorSystem(uint8_t[3] positivePins, uint8_t[3] negativePins, char[3] oPorts){//initialise
		uint8_t status = 0; //0 means that the function assigned pins correctly
		for (int i = 0; i<3; i++){
			switch (oPorts[i]){ //assign the two pins on the required port
				case 'b': DDRB |= (1<<positivePins[i]) | (1<<negativePins[i]);
				case 'B': DDRB |= (1<<positivePins[i]) | (1<<negativePins[i]);
				case 'c': DDRC |= (1<<positivePins[i]) | (1<<negativePins[i]);
				case 'C': DDRC |= (1<<positivePins[i]) | (1<<negativePins[i]);
				case 'd': DDRD |= (1<<positivePins[i]) | (1<<negativePins[i]);
				case 'D': DDRD |= (1<<positivePins[i]) | (1<<negativePins[i]);
				default: status = i; //pin assignments failed!
			}
		}
		
	}
	
	
	
};


uint8_t motorInit(uint8_t pin, uint8_t pinN, char oPort); //return status for tracking purposes




int main(void)
{
	motorSystem(4,5,'b');
	
	while(1)
	{
		//TODO:: Please write your application code
	}
}



void motorControl(char direction, int speed){
	
}