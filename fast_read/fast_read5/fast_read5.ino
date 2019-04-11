const byte adcPin = 0;
volatile int adcReading;
volatile boolean adcDone;
boolean adcStarted;

int vref = 128;

int counter = 0;
int  t = 0;
int  told = 0;

void setup ()
{
  Serial.begin (9600);
  ADCSRA =  bit (ADEN);                      // turn ADC on
  //ADCSRA |= bit (ADPS0) |  bit (ADPS1) | bit (ADPS2);  // Prescaler of 128
  //ADCSRA |= (1 << ADPS2);                   // 16 prescaler for 76.9 KHz
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz
  //ADCSRA |= bit (ADPS0);                               //   2


  ADMUX  =  bit (REFS0) | (adcPin & 0x07) | (1 << ADLAR);    // AVcc and select input port and set 8 bit resolution
  
  ADCSRA |= (1 << ADATE); // enable auto trigger run untill (0 << ADATE)
  ADCSRA |= bit (ADSC) | bit (ADIE); // start ADC measurements && enable interrupts when measurement complete
}  // end of setup

// ADC complete ISR
ISR (ADC_vect)
{
  adcReading = ADCH;
  counter++;
  if (counter > 999) {
    Serial.println (adcReading);
    
    t = micros() - told; // calculate elapsed time
    Serial.print("Sampling frequency: ");
    Serial.print((float)1000000 / t);
    Serial.println(" KHz");
    delay (500);
    counter = 0;
    told = micros();
    ADCSRA |= bit (ADSC) | bit (ADIE);
  } else ADCSRA |= bit (ADSC) | bit (ADIE);
  //adcDone = true;
}  // end of ADC_vect

void loop ()
{
  //
  //  // if last reading finished, process it
  //  if (adcDone)
  //  {
  //
  //    //if (adcReading > vref) Serial.println (adcReading);
  //    if (counter > 999) {
  //      // do something with the reading, for example, print it
  //      Serial.println (adcReading);
  //
  //
  //      t = micros() - told; // calculate elapsed time
  //
  //      Serial.print("Sampling frequency: ");
  //      Serial.print((float)1000000 / t);
  //      Serial.println(" KHz");
  //
  //      delay (500);
  //      counter = 0;
  //      told = micros();
  //    } else if (counter < 999) counter = counter;
  //    adcDone = false;
  //    ADCSRA |= bit (ADSC) | bit (ADIE);
  //  }

  // do other stuff here

}  // end of loop
