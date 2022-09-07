#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>

//MPU6050 mpu;

Servo place_pen;
//Servo graphicator;

boolean turn_on = false; //Input for the turning on system
boolean change_disk = false; //Input for the disk change
boolean initii = false; //Input for the initii sequence
boolean vmax = false; //Input for the vmax 
boolean vmin = false; //Input for the vmin

const char NO_POWER = '0';
const char IDLE_ = '1';
const char CHANGE_DISK = '2';
const char initiiIATE = '3';
const char VMAX = '4';
const char VMIN = '5';

char current_phase;

int minimum = 0;
int maximum = 180;

int servo1 = 0;

void setup() {
  // put your setup code here, to run once:
  DDRD &= B11100000; //PORTD pin [4:0] configured as inputs 
  DDRD |= B11000000; //PORTD pin [7:6] configured as outputs
  place_pen.attach(5); //Set the PWM port for moving the pen 
  //graphicator.attach(6);//Set the PWM port for graphicating the meassurements
  delay(50);
  
  // pinMode(6,OUTPUT); //IDLE_
  // pinMode(7,OUTPUT);//Set the enabling port for the step motor, CHANGE_DISK
  // pinMode(8,OUTPUT);//Set the reset port the step motor, 
  
  // pinMode(10,INPUT);//Set the MSB from the step motor indicator
  // pinMode(11,INPUT);//Set the LSB from the step motor indicator
  
  //DDRB |= B0000101; //PORTB pin 2 & 0 configured as outputs
  DDRB |= B00001101; //PORTB pin [3:2] & 0 configured as outputs
  delay (50); 
  PORTB &= B11110010; //PORTB pin [3:2] & 0 set as 0
  PORTD &= B00111111; //PORTD pin [7:6] set as 0
  current_phase = NO_POWER; //By this state everything will begin 
}

void loop() {
  switch (current_phase){
	case NO_POWER:	//State for the beginning 
			if(PIND & B00000001){
				current_phase = IDLE_; 
				turn_on = true;
				delay(50);
			}
			else{
				current_phase = NO_POWER;
				turn_on = false;
				change_disk = false; 
				initii = false; 
				vmax = false; 
				vmin = false; 
				delay(50);
			}
			break;
	case IDLE_:  //State for selecting which action may be done 
			delay(1000);
			if(PIND & B00000001){
				current_phase = NO_POWER;
				delay(50);
			}
			else if(PIND & B00000010){
				current_phase = CHANGE_DISK;
				delay(50);
			}
			else if(PIND & B00000100){
				current_phase = initiiIATE;
				delay(50);
			}
			else{
				current_phase = IDLE_;
				turn_on = true;
				change_disk = false; 
				initii = false; 
				vmax = false; 
				vmin = false; 
				delay(50);
			}
			break;
	case CHANGE_DISK: //This state will the pen be retired from the disk
			delay(1000);
			if(PIND & B00000001){
				current_phase = NO_POWER;
				delay(50);
			}
			else if(PIND & B00000010){
				current_phase = IDLE_;
				delay(50);
			}
			else if(PIND & B00000100){
				current_phase = initiiIATE;
				delay(50);
			}
			else{
				current_phase = CHANGE_DISK;
				change_disk = true;
				turn_on = false;
				initii = false; 
				vmax = false; 
				vmin = false; 
				delay(50);
			}
			break;
	case initiiIATE: //This state will begin the sequence of the register
			delay(1000);
			if(PIND & B00000001){
				current_phase = NO_POWER;
				delay(50);
			}
			else if(PIND & B00000100){
				current_phase = IDLE_;
				delay(50);
			}
			else if(PIND & B00001000){
				current_phase = VMAX;
				delay(50);
			}
			else if(PIND & B00010000){
				current_phase = VMIN;
				delay(50);
			}
			else{
				current_phase = initiiIATE;
				change_disk = false;
				turn_on = false;
				initii = true; 
				vmax = false; 
				vmin = false; 
				delay(50);
			}
			break;
	case VMAX: //This state will make the pen go at its maximum value
			delay(1000);
			if(PIND & B00000001){
				current_phase = NO_POWER;
				delay(50);
			}
			else if(PIND & B00001000){
				current_phase = initiiIATE;
				delay(50);
			}
			else{
				current_phase = VMAX;
				change_disk = false;
				turn_on = false;
				initii = false; 
				vmax = true; 
				vmin = false; 
				delay(50);
			}
			break;
	case VMIN: //This state will make the pen go at its minimum value
			delay(1000);
			if(PIND & B00000001){
				current_phase = NO_POWER;
				delay(50);
			}
			else if(PIND & B00001000){
				current_phase = initiiIATE;
				delay(50);
			}
			else{
				current_phase = VMIN;
				change_disk = false;
				turn_on = false;
				initii = false; 
				vmax = false; 
				vmin = true; 
				delay(50);
			}
			break;
  }
  
  delay(100);
  
  if(turn_on){
	  //PORTD |= B00100000;
	  PORTD |= B01000000;
  }
  else{
	  //PORTD &= B11011111;
	  PORTD &= B10111111;
  }
  
  if(change_disk){
	  //PORTD |= B01000000;
	  PORTD |= B10000000;
	  servo1 = 90;                                                                                                                     
  }
  else{
	  //PORTD &= B10111111;
	  PORTD &= B01111111;
	  servo1 = 0;
  }

  if(initii){
	  PORTB |= B00001000;
	  //PORTD |= B10000000;
  }
  else{
	  PORTB &= B11110111;
	  //PORTD &= B01111111;
  }
  
  if(vmax){
	  PORTB |= B00000001;
  }
  else{
	  PORTB &= B11111110;
  }
  
  if(vmin){
	  PORTB |= B00000100;
  }
  else{
	  PORTB &= B11111011;
  }
  
  place_pen.write(servo1);
  delay(15);
  
}