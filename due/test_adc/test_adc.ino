unsigned long start_time;
unsigned long stop_time;
unsigned long values[1000];
unsigned int duty = 0;

void setup() {
  Serial.begin(9600);

  ADC->ADC_CHDR = 0xFFFF; //Disable all channel to be shore | pdf page 1351
  ADC->ADC_MR |= 0x80;  //set free running mode on ADC and 12-bit resolution | pdf page 1345
  ADC->ADC_CHER = 0x80; //enable ADC on pin A0 | pdf page 1350
}

void loop() {
 while ((ADC->ADC_ISR & 0x80) == 0); // wait for conversion
    Serial.print("adc ");
    Serial.print(7);
    Serial.print(" : ");
    Serial.println(ADC->ADC_CDR[7]);

 while ((ADC->ADC_ISR & 0x40) == 0); // wait for conversion
    Serial.print("adc ");
    Serial.print(6);
    Serial.print(" : ");
    Serial.println(ADC->ADC_CDR[6]); 

 while ((ADC->ADC_ISR & 0x20) == 0); // wait for conversion
    Serial.print("adc ");
    Serial.print(5);
    Serial.print(" : ");
    Serial.println(ADC->ADC_CDR[5]); 

 while ((ADC->ADC_ISR & 0x10) == 0); // wait for conversion
    Serial.print("adc ");
    Serial.print(4);
    Serial.print(" : ");
    Serial.println(ADC->ADC_CDR[4]);
    
  delay(1000);
  exit(0);
}
