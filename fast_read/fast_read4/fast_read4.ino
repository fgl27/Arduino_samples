const byte adcPin = 0;
volatile int adcReading;
volatile boolean adcDone;
boolean adcStarted;

void setup ()
{
  Serial.begin (9600);
  ADCSRA =  bit (ADEN);                      // turn ADC on
  //ADCSRA |= bit (ADPS0) |  bit (ADPS1) | bit (ADPS2);  // Prescaler of 128
  ADCSRA |= bit (ADPS0);                               //   2
  
  ADMUX  =  bit (REFS0) | (adcPin & 0x07) | (1 << ADLAR);    // AVcc and select input port and set 8 bit resolution

}  // end of setup

// ADC complete ISR
ISR (ADC_vect)
  {
  adcReading = ADCH;
  adcDone = true;  
  }  // end of ADC_vect
  
void loop ()
{
  // if last reading finished, process it
  if (adcDone)
    {
    adcStarted = false;

    // do something with the reading, for example, print it
    Serial.println (adcReading);
    delay (500);

    adcDone = false;
    }
    
  // if we aren't taking a reading, start a new one
  if (!adcStarted)
    {
    adcStarted = true;
    // start the conversion
    ADCSRA |= bit (ADSC) | bit (ADIE);
    }    
  
  // do other stuff here

}  // end of loop
