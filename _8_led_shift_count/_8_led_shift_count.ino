int latchPin = 8; //Pin connected to ST_CP of 74HC595, latch pin
int clockPin = 12; //Pin connected to SH_CP of 74HC595, CLK pin
int dataPin = 11; //Serial input to shift resister
int SRCLR = 7; //Clear Pin (HIGH = leave it, LOW = wipe output)
int led_9 = 1; //Center LED, connected directly to pin with 330 ohm resistor
int GND_1 = 6; //Bottom Layer Ground
int GND_2 = 5; //Middle Layer Ground
int GND_3 = 4; //Top Layer Ground

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(led_9, OUTPUT);  
  pinMode(SRCLR, OUTPUT);
  pinMode(GND_1, OUTPUT); //Connected to 2n2222 transistor (HIGH = open circuit, LOW = closed circuit)
  pinMode(GND_2, OUTPUT); //Connected to 2n2222 transistor (HIGH = open circuit, LOW = closed circuit)
  pinMode(GND_3, OUTPUT); //Connected to 2n2222 transistor (HIGH = open circuit, LOW = closed circuit)
}

void loop() {
  boolean arr[8] = {0,0,0,0,0,0,0,0};
  digitalWrite(SRCLR, HIGH); //Don't clear
  for( int i = 0; i < 3; i++ ){
    if( i == 0 ){
      digitalWrite( GND_3, LOW );
      digitalWrite( GND_2, LOW );
      digitalWrite( GND_1, HIGH );
    }
    if( i == 1 ){
      digitalWrite( GND_3, LOW );
      digitalWrite( GND_1, LOW );
      digitalWrite( GND_2, HIGH );
    }
    if( i == 2 ){
      digitalWrite( GND_1, LOW );
      digitalWrite( GND_2, LOW );
      digitalWrite( GND_3, HIGH );
    }
    for (int j = 0; j <= 8; j++) {
      if( j == 8 ) {
        digitalWrite( led_9, HIGH );
        digitalWrite(latchPin, LOW); //ground latchPin and hold low for as long as you are transmitting
        shiftOut(dataPin, clockPin, MSBFIRST, 0);
        digitalWrite(latchPin, HIGH); //return the latch pin high to signal chip that it
        delay(300);
        digitalWrite( led_9, LOW );
        continue;
      }
      arr[j] = 1;
      digitalWrite(latchPin, LOW); //ground latchPin and hold low for as long as you are transmitting
      shiftOut(dataPin, clockPin, MSBFIRST, array_to_byte( arr ) );
      digitalWrite(latchPin, HIGH); //return the latch pin high to signal chip that it
      delay(300);
      arr[j] = 0;
    }
//    digitalWrite(GND_TRANS, LOW);
//    delay( 500 );
//    digitalWrite(GND_TRANS, HIGH);
    
//    digitalWrite(latchPin, LOW);
//    shiftOut(dataPin, clockPin, MSBFIRST, 64);
    //return the latch pin high to signal chip that it
    //no longer needs to listen for information
//    digitalWrite(latchPin, HIGH);    
//    delay(1000);
//    digitalWrite(GND_TRANS, LOW);
//    delay( 500 );
//    digitalWrite(GND_TRANS, HIGH);
    
//    digitalWrite(latchPin, LOW);
//    shiftOut(dataPin, clockPin, MSBFIRST, 12);
    //return the latch pin high to signal chip that it
    //no longer needs to listen for information
//    digitalWrite(latchPin, HIGH);    
//    delay(2000);
  }
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
