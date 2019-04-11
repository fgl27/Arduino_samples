// Prescaler accuracy test

int analogReadP(uint8_t pin, uint8_t prescale=7)
{
    uint8_t ADCSRA_TMP = ADCSRA;
    prescale = constrain(prescale, 2, 7);
    ADCSRA = (ADCSRA | 0x07) & (0xF8 | prescale);
    int value = analogRead(pin);
    ADCSRA = ADCSRA_TMP;
    return value;
}

void setup ()
{
  Serial.begin (9600);
  Serial.println ();
  
  ADMUX  =  bit (REFS0) | (0 & 0x07) | (1 << ADLAR);

  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); // clear prescaler bits

  // uncomment as required

    ADCSRA |= bit (ADPS0);                               //   2
  //  ADCSRA |= bit (ADPS1);                               //   4
  //  ADCSRA |= bit (ADPS0) | bit (ADPS1);                 //   8
  //  ADCSRA |= bit (ADPS2);                               //  16
  //  ADCSRA |= bit (ADPS0) | bit (ADPS2);                 //  32
  //  ADCSRA |= bit (ADPS1) | bit (ADPS2);                 //  64
  //  ADCSRA |= bit (ADPS0) | bit (ADPS1) | bit (ADPS2);   // 128

}  // end of setup

const int ITERATIONS = 1000;
unsigned long totals [6];
const byte lowPort = 0;
const byte highPort = 0;

void loop ()
{
  for (int whichPort = lowPort; whichPort <= highPort; whichPort++)
    totals [whichPort - lowPort] = 0;

  unsigned long startTime = micros ();
  for (int i = 0; i < ITERATIONS; i++)
  {
    for (int whichPort = lowPort; whichPort <= highPort; whichPort++)
    {
      int result = analogRead (whichPort);
      totals [whichPort - lowPort] += result;
    }
  }
  unsigned long endTime = micros ();

  for (int whichPort = lowPort; whichPort <= highPort; whichPort++)
  {
    Serial.print ("Analog port = ");
    Serial.print (whichPort);
    Serial.print (", average result = ");
    Serial.println (totals [whichPort - lowPort] / ITERATIONS);
  }
  Serial.print ("Time taken = ");
  Serial.print (endTime - startTime);
  Serial.println ();
  Serial.print("Sampling frequency: ");
  Serial.print((float)1000000 / (endTime - startTime));
  Serial.println(" KHz");

  Serial.println ();
  Serial.flush ();
  exit (0);
}  // end of loop
