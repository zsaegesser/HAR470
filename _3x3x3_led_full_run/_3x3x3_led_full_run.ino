#include <string.h>
int latchPin = 8; //Pin connected to ST_CP of 74HC595, latch pin
int clockPin = 12; //Pin connected to SH_CP of 74HC595, CLK pin
int dataPin = 11; //Serial input to shift resister
int SRCLR = 7; //Clear Pin (HIGH = leave it, LOW = wipe output)
int led_9 = 1; //Center LED, connected directly to pin with 330 ohm resistor
int GND_1 = 6; //Bottom Layer Ground
int GND_2 = 5; //Middle Layer Ground
int GND_3 = 4; //Top Layer Ground
unsigned long time;
unsigned long myClock;
boolean reset;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(led_9, OUTPUT);  
  pinMode(SRCLR, OUTPUT);
  pinMode(GND_1, OUTPUT); 
  pinMode(GND_2, OUTPUT); 
  pinMode(GND_3, OUTPUT); 
  myClock = 0;
  reset = 0;
}

void loop() {
  time = millis();
  if( reset == 1 ){
    reset = 0;
    myClock = time;
  }
  digitalWrite( SRCLR, HIGH );
  boolean diamond[3][9] = { {0,0,0,0,1,0,0,0,0},
                        {0,1,0,1,1,1,0,1,0},
                        {0,0,0,0,1,0,0,0,0}
  };
  boolean full_cube[3][9] = { {1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1},
                        {1,1,1,1,1,1,1,1,1}
  };
  boolean rev_diamond[3][9] = { {1,1,1,1,0,1,1,1,1},
                        {1,0,1,0,0,0,1,0,1},
                        {1,1,1,1,0,1,1,1,1}
  };  
  if( time - myClock < 1000 ){
    run_round( diamond );    
  }
  else if( (time - myClock >= 1000) && (time - myClock < 2000) ){
    run_round( rev_diamond );
  }
  else {
    reset = 1;
  }
}

void run_round( boolean array[3][9] ){

  for( int i = 0; i < 3; i++ ){
    boolean temp[8];
    memcpy(temp, array[i], sizeof(temp));
//    for( int k=0; k<8; k++ ){
//      temp[k] = array[i][k];
//    }
//    digitalWrite( SRCLR, LOW );
    reset_leds();
    digitalWrite(latchPin, LOW); //ground latchPin and hold low for as long as you are transmitting
    shiftOut(dataPin, clockPin, MSBFIRST, array_to_byte( temp ) );
    digitalWrite(latchPin, HIGH); //return the latch pin high to signal chip that it
    ground_row( i );
    if( array[i][8] ){
      digitalWrite( led_9, HIGH );
    }
    else{
      digitalWrite( led_9, LOW );
    }
//    digitalWrite( SRCLR, HIGH );
//    delay( 200 );
  }
  return;
}

void ground_row( int row_num ){
  if( row_num == 0 ){
      digitalWrite( GND_3, LOW );
      digitalWrite( GND_2, LOW );
      digitalWrite( GND_1, HIGH );
    }
  if( row_num == 1 ){
    digitalWrite( GND_3, LOW );
    digitalWrite( GND_1, LOW );
    digitalWrite( GND_2, HIGH );
  }
  if( row_num == 2 ){
    digitalWrite( GND_1, LOW );
    digitalWrite( GND_2, LOW );
    digitalWrite( GND_3, HIGH );
  }
  return;
}

void reset_leds(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0 );
  digitalWrite(latchPin, HIGH);
  digitalWrite( led_9, LOW );
  digitalWrite( GND_1, LOW );
  digitalWrite( GND_2, LOW );
  digitalWrite( GND_3, LOW );  
}

byte array_to_byte( boolean array[8] ){
  byte result = 0; 
  for( int i = 0; i < 8; i++ ) {
    if( array[i] ) {
      result = result | (1 << i);
    }
  }
  return result;
}
