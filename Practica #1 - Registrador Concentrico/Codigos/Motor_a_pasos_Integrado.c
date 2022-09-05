int dly = 11;
int count = 0;

boolean initi = false;
boolean end_seq = false;
boolean reset = false;

void setup() {
  DDRB |= B00001111; //PORTB [3:0] pins configured as ONE HOT outputs 
  DDRD &= B00111111; //Configured PORTD [7:6] pins as inputs for enabling and reset
  DDRD |= B00110000; //Configured PORTD [5:4] pins as indicative outputs of the number of lap      
  delay(50);
  PORTB &= B11110000;
}

void loop(){
    
  if(PIND & B10000000){
    initi = true;
	delay(1);
  }
  else{
	initi = false;
	delay(1);
  }
  
  if(PIND & B01000000){
	reset = true;  
	delay(1);
  }
  else{
	reset = false;  
	delay(1);
  }
  
  if(count > 2048){ //Once the four laps have been completed the disk will stop turnning
	end_seq = true;
	delay(1);
  }
  else{
	end_seq = false;
	delay(1);
  }
  
  if(initi && !end_seq){ //One hot sequence for the step motor
	  PORTB &= B00000001;
	  PORTB |= B00000001;
	  delay(dly);
	  
	  PORTB &= B00000010;
	  PORTB |= B00000010;
	  delay(dly);
	  
	  PORTB &= B00000100;
	  PORTB |= B00000100;
	  delay(dly);
	  
	  PORTB &= B00001000;
	  PORTB |= B00001000;
	  delay(dly);
	  
	  if(reset){
	    count = 0;
		delay(1);
	  }
	  else{
	    count = count + 1;
		delay(1);
	  }
  }
  
  if(count < 512){
    PORTD &= B11001111; //PORTD[5:4] = 00
	delay(1);
  }
  else if(count < 1024){
    PORTD |= B00010000; //PORTD[5:4] = 01
	delay(1);
  }
  else if(count < 1536){
	PORTD &= B11101111;
	PORTD |= B00100000; //PORTD[5:4] = 10
	delay(1);
  }
  else{
	PORTD |= B00110000; //PORTD[5:4] = 11  
	delay(1);
  }
}