
/*
  PIO with polling input
  Example  which uses a tactile switch that when pressed dims a normally lit onboard LED.
  The switch is configured as an input on PC12/DUE PIN N° 51 and the onboard LED is located on PB27/DUE PIN N°13.
*/


void setup() {
  Serial.begin(9600);
  //We need to associate clock with Port C so we can read input
  //A list of Peripheral ID's can be found on Page 37 in the datasheet.

  PMC->PMC_PCER0 = PMC_PCER0_PID13; // PIOC power on

  //Enable PC15 and PC12 (Peripheral Enable Register)
  PIOC->PIO_PER = PIO_PC12 | PIO_PC15;

  //Set C15 as output (Output Enable Register)
  PIOC->PIO_OER = PIO_PC15;

  //Set C12 as input (Ouput Disable Register)
  PIOC->PIO_ODR = PIO_PC12;

  //Disable pull-up on both pins (Pull Up Disable Register)
  PIOC->PIO_PUDR = PIO_PC15;
  PIOC->PIO_PUDR = PIO_PC12;
}


void loop() {
  //Very simple polling routine that checks the Pin Data Register's status
  //and sets the state of the LED accordingly.

  if ((PIOC->PIO_PDSR & PIO_PC12) == PIO_PC12) {
    PIOC->PIO_SODR = PIO_PC15;  //Set Ouput Data Register
    Serial.println("ON");
  } else {
    PIOC->PIO_CODR = PIO_PC15;  //Clear Output Data Register
    Serial.println("OFF");
  }
  delay(10);
//exit(0);
}
