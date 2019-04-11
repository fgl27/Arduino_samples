unsigned long start_time;
unsigned long stop_time;
unsigned long values[1000];
unsigned int duty = 100;
unsigned int i;

void setup() {
  Serial.begin(9600);

  ADC->ADC_CHDR = 0xFFFF; //Disable all channel to be shore | pdf page 1351
  ADC->ADC_MR |= 0x80;  //set free running mode on ADC and 12-bit resolution | pdf page 1345
  ADC->ADC_CHER = 0xF0; //enable ADC on pin A0 | pdf page 1350
  delay(1000);
}

void loop() {

  start_time = micros();

  Serial.print("Time before: ");
  Serial.println(start_time);
  for (int x = 0; x < 1000; x++) {
    for (i = 0; i < 1000; i++) {
      while ((ADC->ADC_ISR & 0xF0) == 0); // wait for conversion
      values[i] = ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4]; //get values
    }
  }
  stop_time = micros();
  Serial.print("Time after: ");
  Serial.println(stop_time);

  Serial.print("Total time: ");
  Serial.println(stop_time - start_time);
  Serial.print("Average time per conversion: ");
  Serial.println((float)(stop_time - start_time) / 1000000);

  delay(1000);
  exit(0);
}
