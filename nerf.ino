// Definition of interrupt names
#include <avr/io.h>
// ISR interrupt service routine
#include <avr/interrupt.h>
const int pAIN1 = 7;  // AIN1 at Pin7
volatile boolean iflag = true;
volatile int idx;
uint32_t last_time;
volatile int last_speed;
volatile bool enable = true;
volatile int e;
volatile int r;

// Install the interrupt routine for ACOMP
ISR(ANALOG_COMP_vect) {
  if (ACSR & (1 << ACO))  // ACO is set?
  {
  } 
  else 
  {
    if(true == enable)
    {
      idx++;
      enable = false;
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(pAIN1, INPUT);
  cli();
  ADCSRA &= ~(1 << ADEN);            // ADC disabled
  ADCSRB |= ~(1 << ACME);            // AMUX enabled
  ACSR = (1 << ACBG) | (1 << ACIE);  // ACOMP Interrupt enabled
  DIDR1 = (1 << AIN1D) | (1 << AIN0D);
  sei();
  Serial.print("ADCSRA: ");
  Serial.println(ADCSRA, HEX);
  Serial.print("ADCSRB: ");
  Serial.println(ADCSRB, HEX);
  Serial.print("ACSR: ");
  Serial.println(ACSR, HEX);
  Serial.print("DIDR1: ");
  Serial.println(DIDR1, HEX);
  Serial.println("Setup finished.");

  uint32_t last_time = millis();
  
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  pinMode(10, INPUT_PULLUP); // bullet counter
  pinMode(11, INPUT_PULLUP); //trigger
  pinMode(12, INPUT_PULLUP); // flywheel

  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);

   pinMode(A1, INPUT_PULLUP); // one shot
   pinMode(A2, INPUT_PULLUP); // series
  pinMode(A3, INPUT_PULLUP); // full auto
  }
void loop() 
{

    if(millis() > (last_time + 1))
    {
      enable = true;      
    }
  
    if(millis() > (last_time + 50))
    {
      last_time = millis();
      last_speed = idx * 20;
      idx = 0;

      e = 250 - last_speed;
      r = e*2;

      if(r < 0)
      {
        r = 0;
      }
      
      if(r > 255)
      {
        r = 255;
      }
      
      analogWrite(6, r);            
      // Serial.println(last_speed);  // iflag controls serial output
      // Serial.println(r);  // iflag controls serial output

      Serial.print(digitalRead(A1));
      Serial.print(digitalRead(A2));
      Serial.print(digitalRead(A3));
      Serial.print(digitalRead(10));
      Serial.print(digitalRead(11));
      Serial.println(digitalRead(12));
    }

    // analogWrite(6, 50);   
}