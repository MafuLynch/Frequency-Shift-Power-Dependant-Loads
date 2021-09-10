unsigned int outputvalue;
unsigned int previous = 0;

void setup() { 
  pinMode(9, OUTPUT);                               // Set digital pin 9 (D9) to an output
  TCCR1A = _BV(COM1A1) | _BV(WGM11);                // Enable the PWM output OC1A on digital pins 9
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12);     // Set fast PWM and prescaler of 256 on timer 1
  ICR1 = 62499;                                     // Set the PWM frequency to 1Hz: 16MHz/(256 * 1Hz) - 1 = 62499
  OCR1A = 6249;                                     // Set the duty-cycle to 10%: 62499 / 10 = 6249
  TCNT1  = 0;   // Set Timer1 preload value to 0 (reset)
  TIMSK1 = 1;   // enable Timer1 overflow interrupt

  EIFR |= 1;  // clear INT0 flag
  attachInterrupt(0, timer1_get, FALLING);  // enable external interrupt (INT0)
}

uint16_t tmr1 = 0;
float period, frequency;
 
void timer1_get() {
  tmr1 = TCNT1 - previous;
  previous = TCNT1;
  //TCNT1  = 0;   // reset Timer1
  
}
 
ISR(TIMER1_OVF_vect) {  // Timer1 interrupt service routine (ISR)
  tmr1 = 0;
  previous = 0;
}

void loop(){
  
  // save current Timer1 value
  uint16_t value = tmr1;
  // calculate signal frequency which is = 1/period ; or = MCU_CLK/(Prescaler * Timer_Value)
  // since using full bridge rectifier we measure half period, so f=1/2*p
  if(value == 0)
    frequency = 0;   // aviod division by zero
  else
    frequency = 16000000.0/(2*(256UL*value));

  if (frequency > 51) {
    outputvalue = 62499u;
  } else if (frequency < 50.5) {
    outputvalue = 0;
  } else {
    outputvalue = 2 * 62499ul * (frequency - 50.5);
  }
  
  OCR1A = outputvalue;

}
