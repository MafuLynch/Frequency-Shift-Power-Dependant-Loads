int triacPin=3;
float Fdelay;
int Tdelay;
 
void setup(void) {

  pinMode(4,OUTPUT);
  digitalWrite(4,LOW);
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);

  
 
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
    if (frequency>=50.50 && frequency <51){
      Fdelay=9200*(1+(50.5-frequency)*2);  //linear interpolation for delay
      Tdelay=int(Fdelay);
      delayMicroseconds(Tdelay);
      digitalWrite(4,HIGH);
      delayMicroseconds(50);   //revisar que es posible que este delay de 100uS este activando el triac en la siguiente onda a partir de t'=0. Tdelay + delay de codigo + delay 100uS = 9800+100+algo muy chico --> 9900uS + extra y la media onda en 50.5 son 9.9miliseg
      digitalWrite(4,LOW);
      }

}
 
ISR(TIMER1_OVF_vect) {  // Timer1 interrupt service routine (ISR)
  tmr1 = 0;
}
 

void loop() {
 
  // save current Timer1 value
  uint16_t value = tmr1;
  // calculate signal frequency which is = 1/period ; or = MCU_CLK/(Prescaler * Timer_Value)
  

  if (frequency>=51.0){ //if frequency over 51Hz triac should be always on
    digitalWrite(4,HIGH);
  }

  if (frequency<50.5){ //If low frequency, deactivate triac
    digitalWrite(4,LOW);
    digitalWrite(12,LOW);
  }

  if (frequency>50.60){ //Start fan when 20% output power
    digitalWrite(12,HIGH);
    }
 
}
 
