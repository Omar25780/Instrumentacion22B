boolean turn_on = false;
boolean change_disk = false;
boolean initiate = false;
boolean vmax = false;
boolean vmin = false;
char count = 0;

void setup() {
  // put your setup code here, to run once:
  DDRD = B1110000;
  DDRB |= B0000011;
  delay (50); 
  PORTB &= B11111100;
  PORTD &= B00011111;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(PIND & B00000001){
    turn_on = true;
    delay(50);
  }
  delay(1000);
  while(turn_on == true){ //IDLE
  PORTD |=  B00100000;
    if(PIND & B00000001){ 
     turn_on = false;
     delay(1000);
    }
    else if(((PIND & B00000010)&&(turn_on == true)&&(initiate == false))||(change_disk == true)){//CHANGE THE DISK
     delay(50);
     change_disk = true;
     PORTD &=  B11011111;
     PORTD |=  B01000000;
     delay(1000);
     if(PIND & B00000100){
     change_disk = false;
     initiate = true;
     delay(1000);
     }
    }
    else if((((PIND & B00000100)&&(turn_on == true))||(initiate == true))&&((vmax == false)&&(vmin == false))){//INITIATE
     initiate = true;
     PORTD |= B10000000;
     PORTD &= B10011111;
     PORTB &= B11111100;
     if(PIND & B00001000){
      vmax = true;
      delay(1000);
     }
     if(PIND & B00010000){
      vmin = true;
      delay(1000);
     }
    }
    else if((vmax == true)&&(vmin == false)){//VMAX
     PORTB |= B00000001;
     PORTD |=  B10000000;
     if(PIND & B00001000){
      vmax = false;
      delay(1000);
     }
    }
    else if((vmin == true)&&(vmax == false)){//VMIN
     PORTB |= B00000010;
     PORTD |=  B10000000;
     if(PIND & B00010000){
      vmin = false;
      delay(1000);
     }
    }
    else{
      PORTB &= B11111100;
      PORTD &= B00111111;
      PORTD |= B00100000;
    }
  } 
  PORTB &= B11111100;
  PORTD &= B00011111;
  boolean change_disk = false;
  boolean initiate = false;
  boolean vmax = false;
  boolean vmin = false;
}