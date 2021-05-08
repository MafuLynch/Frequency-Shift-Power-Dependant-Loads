
/*************************************************************************
 * 
 * Arduino frequency counter.
 * This is a free software with NO WARRANTY.
 * USE AT YOUR OWN RISK!
 * https://simple-circuit.com/
 *
 ************************************************************************/
 
#include <LiquidCrystal.h>    // include Arduino LCD library
 
// LCD module connections (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
int triacPin=3;
float Fdelay;
int Tdelay;
bool detectado = false;
 
void setup(void) {

  Serial.begin(9600); 
  lcd.begin(16, 2);      // set up the LCD's number of columns and rows
  lcd.setCursor(0, 0);
  lcd.print("Freq =");
  lcd.setCursor(0, 1);
  lcd.print("Peri =");
  pinMode(4,OUTPUT);
  digitalWrite(4,LOW);
 
  // Timer1 module configuration
  TCCR1A = 0;
  TCCR1B = 2;   // enable Timer1 module with 1/8 prescaler ( 2 ticks every 1 us)
  TCNT1  = 0;   // Set Timer1 preload value to 0 (reset)
  TIMSK1 = 1;   // enable Timer1 overflow interrupt
 
  EIFR |= 1;  // clear INT0 flag
  attachInterrupt(0, timer1_get, FALLING);  // enable external interrupt (INT0)
}
 
uint16_t tmr1 = 0;
float period, frequency;
 
void timer1_get() {
  tmr1 = TCNT1;
  TCNT1  = 0;   // reset Timer1
  //Serial.println("interrupt");
  detectado=true;
}
 
ISR(TIMER1_OVF_vect) {  // Timer1 interrupt service routine (ISR)
  tmr1 = 0;
}
 
// main loop
void loop() {
 
  // save current Timer1 value
  uint16_t value = tmr1;
  // calculate signal period in milliseconds
  // 8.0 is Timer1 prescaler and 16000 = MCU_CLK/1000
  period = 8.0 * value/16000;
  // calculate signal frequency which is = 1/period ; or = MCU_CLK/(Prescaler * Timer_Value)
  if(value == 0)
    frequency = 0;   // aviod division by zero
  else
    frequency = 16000000.0/(2*(8UL*value));

  if (detectado){  
    if (frequency>=50.5 && frequency <51){
      Fdelay=9500+(50.5-frequency)*2*9490;
      Tdelay=int(Fdelay);
      Serial.println(Tdelay);
      delayMicroseconds(Tdelay);
      digitalWrite(4,HIGH);
      delayMicroseconds(100);
      digitalWrite(4,LOW);
    }
    detectado=false;
  } 
  if (frequency>=51){ //if frequency over 51Hz triac should be always on
    digitalWrite(4,HIGH);
  }
  if (frequency<50.5){ //If low frequency, deactivate triac
    digitalWrite(4,LOW)
  }

  Serial.println(frequency);
  
  //delay(500);
 
}
 
// end of code.
