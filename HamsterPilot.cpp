/*
 * Hammond.cpp
 *
 * Created: 4/15/2025 2:25:26 PM
 * Author : Benjamin Betancourt
 *
 *
 * Remember to merge with HamsterPilot.cpp for ease of transportation
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 


class motorSystem {
	/*
	* Motor 0 and 1 are the front and side motors, motor 2 is the back motor
	*
	* The positive pins enabled while negative pins disabled mean that the wheels move forward
	* Negative enabled while positive pins disabled means that the wheels move backwards
	* This intuition will be used for directional control
	*
	* Status codes:
	* 0 = All is dandy
	* 1 = error with motor 0 initialization
	* 2 = error with motor 1 initialization
	* 3 = error with motor 2 initialization
	* -1 = error with pin assignment (Using reserved pin such as PB5 (on board LED) or interrupt pins (PD2 and PD3) for motor control
	*/

private:

	int status = 0; //See status codes for more information on functionality

	//storage variables for usage in member functions
	int Ppins[3], Npins[3];
	char motorPorts[3];

public:

	motorSystem(int positivePins[3], int negativePins[3], char oPorts[3]) {//initialize
		int status = 0; //0 means that the function assigned pins correctly
		for (int i = 0; ((i < 3) && (status == 0)); i++) { //break loop if error
			switch (oPorts[i]) { //assign the two pins on the required port, also assign port for later use
			case 'b': DDRB |= (1 << positivePins[i]) | (1 << negativePins[i]); motorPorts[i] = 'B'; break;
			case 'B': DDRB |= (1 << positivePins[i]) | (1 << negativePins[i]); motorPorts[i] = 'B'; break;
			case 'c': DDRC |= (1 << positivePins[i]) | (1 << negativePins[i]); motorPorts[i] = 'C'; break;
			case 'C': DDRC |= (1 << positivePins[i]) | (1 << negativePins[i]); motorPorts[i] = 'C'; break;
			case 'd': DDRD |= (1 << positivePins[i]) | (1 << negativePins[i]); motorPorts[i] = 'D'; break;
			case 'D': DDRD |= (1 << positivePins[i]) | (1 << negativePins[i]); motorPorts[i] = 'D'; break;
			default: status = i + 1; break; //pin assignments failed!
			}
			Ppins[i] = positivePins[i]; Npins[i] = negativePins[i]; //assign class variables from input variables
		}
	}

	void moveForward(int speed) {
		//NOTE: speed may be unused if PWM cannot be figured out in time
		if (status != 0) {
			return; //end early is there are errors from previous executions
		}

		switch (motorPorts[0]) { //motor 0 (front left)
		case 'B': PORTB |= (1 << Ppins[0]); PORTB &= ~(1 << Npins[0]); break; //Enable positive, disable negative (wheel forward)
		case 'C': PORTC |= (1 << Ppins[0]); PORTC &= ~(1 << Npins[0]); break;
		case 'D': PORTD |= (1 << Ppins[0]); PORTD &= ~(1 << Npins[0]); break;
		}

		switch (motorPorts[1]) { //motor 1 (front right)
		case 'B': PORTB |= (1 << Ppins[1]); PORTB &= ~(1 << Npins[1]); break; //Enable positive, disable negative (wheel forward)
		case 'C': PORTC |= (1 << Ppins[1]); PORTC &= ~(1 << Npins[1]); break;
		case 'D': PORTD |= (1 << Ppins[1]); PORTD &= ~(1 << Npins[1]); break;
		}

		switch (motorPorts[2]) { //motor 2 (back)
		case 'B': PORTB &= ~(1 << Ppins[2]); PORTB &= ~(1 << Npins[2]); break; //Disable both positive and negative pins
		case 'C': PORTC &= ~(1 << Ppins[2]); PORTC &= ~(1 << Npins[2]); break; //This wheel is not supposed to move at all
		case 'D': PORTD &= ~(1 << Ppins[2]); PORTD &= ~(1 << Npins[2]); break;
		}

	}

	void moveBackward(int speed) { //note, this just reverses the wheels from the move forward function
		//NOTE: speed may be unused if PWM cannot be figured out in time
		if (status != 0) {
			return; //end early is there are errors from previous executions
		}

		switch (motorPorts[0]) { //motor 0 (front left)
		case 'B': PORTB &= ~(1 << Ppins[0]); PORTB |= (1 << Npins[0]); break; //Enable negative, disable positive (wheel backward)
		case 'C': PORTC &= ~(1 << Ppins[0]); PORTC |= (1 << Npins[0]); break;
		case 'D': PORTD &= ~(1 << Ppins[0]); PORTD |= (1 << Npins[0]); break;
		}

		switch (motorPorts[1]) { //motor 1 (front right)
		case 'B': PORTB &= ~(1 << Ppins[1]); PORTB |= (1 << Npins[1]); break; //Enable negative, disable positive (wheel backward)
		case 'C': PORTC &= ~(1 << Ppins[1]); PORTC |= (1 << Npins[1]); break;
		case 'D': PORTD &= ~(1 << Ppins[1]); PORTD |= (1 << Npins[1]); break;
		}

		switch (motorPorts[2]) { //motor 2 (back)
		case 'B': PORTB &= ~(1 << Ppins[2]); PORTB &= ~(1 << Npins[2]); break; //Disable both positive and negative pins
		case 'C': PORTC &= ~(1 << Ppins[2]); PORTC &= ~(1 << Npins[2]); break; //This wheel is not supposed to move at all
		case 'D': PORTD &= ~(1 << Ppins[2]); PORTD &= ~(1 << Npins[2]); break;
		}

	}

	int getStatus() {
		return status; //check on status easily for debugging codes
	}

};

void uartInit(int baudRate) { // initializes uart for processing with provided baud rate
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // | (1<<RXCIE0)  //uncomment this part for UART recieve inturrupt enable
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //set char size to 8 bits (2 hex characters)

	int UBRR = (16000000 / (16 * baudRate)) - 1; //calculate UBRR (9600 should yield 103)

	//UBRR0H = (UBRR<<8);
	UBRR0L = UBRR;

}
int getUartIn() { //gets UART input 
	return UDR0; //UDR0 should contain the data
}






/*
* Status class variable causes on board LED to flash different patterns based on error codes (See status comments at top of program for more information)
* This will help with code to board debugging
* Codes:
* 0 = LED off
* 1 = 1 second period with 1 quick flash
* 2 = 1 second period with 2 quick flashes
* 3 = 1 second period with 3 quick flashes
* -1 = LED constantly on
*/

int main(void) {
	//array initializations for pins and ports to be used with motors
	int positivePins[3] = { 1,7,0 }; int negativePins[3] = { 0,6,1 }; char ports[3] = { 'B', 'D', 'C' };
	motorSystem movement(positivePins, negativePins, ports);

	int status = movement.getStatus();

		DDRB |= (1 << 5); //set up on board led for debugging

	if (status != 0) { //status has error located somewhere
		switch (status) {
		case 1: while (1) { PORTB ^= (1 << 5); _delay_ms(100); PORTB ^= (1 << 5); _delay_ms(900); } //infinite loop for flashing on board LED as debug code
		case 2: while (1) { PORTB ^= (1 << 5); _delay_ms(100); PORTB ^= (1 << 5); _delay_ms(100); PORTB ^= (1 << 5); _delay_ms(100); PORTB ^= (1 << 5); _delay_ms(700); } //infinite loop for flashing on board LED as debug code
		case 3: while (1) { PORTB ^= (1 << 5); _delay_ms(100); PORTB ^= (1 << 5); _delay_ms(100); PORTB ^= (1 << 5); _delay_ms(100); PORTB ^= (1 << 5); _delay_ms(100); PORTB ^= (1 << 5); _delay_ms(100); PORTB ^= (1 << 5); _delay_ms(500); } //infinite loop for flashing on board LED as debug code
		case -1: while (1) { PORTB ^= (1 << 5); }
		}
	}


	while (1)
	{

	}
}
