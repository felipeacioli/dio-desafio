//#include <avr/interrupt.h>
#include <avr/sleep.h>


#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define ledPin 0
#define redLed 0
#define greenLed 4
volatile int divider=0;
volatile int loopcounter=0;

void setupTimer0() {
  noInterrupts();
  
  CLKPR = 0;
  CLKPR = CLKPCE | CLKPS3;
  
  MCUCR &= ~(SM1|SE|SM0);
  
  
  // Clear registers
  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0 = 0;

  // 100.16025641025641 Hz (1000000/((155+1)*64))
  OCR0A = 255;
  // CTC
  TCCR0A |= (1 << WGM01);
  // Prescaler 64
  TCCR0B |= (1 << CS02) | (1 << CS00);
  // Output Compare Match A Interrupt Enable
  TIMSK |= (1 << OCIE0A);
  //set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here
  //digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
  interrupts();
}

void setup() {
  pinMode(ledPin, OUTPUT);
  DDRB |= 1 << PB4;     //Configure the PORTD4 as output
  setupTimer0();
}


void loop() {
  
  if(loopcounter==0)
  	PORTB ^= (1 << PB4);
  loopcounter++;
  loopcounter%=20000;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();
  sleep_mode();
  
  //digitalWrite(4, digitalRead(4) ^ 1);
 
  /*
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF

  //set_sleep_mode(SLEEP_MODE_IDLE); // sleep mode is set here
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System actually sleeps here

  sleep_disable();                     // System continues execution here when watchdog timed out 
  
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
  */
}


ISR(TIMER0_COMPA_vect) {
  if(divider==0)
    digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
  divider++;
  divider%=100;
  //MCUCR |= SE;
}