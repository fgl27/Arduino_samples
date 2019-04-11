unsigned long start_time;
unsigned long stop_time;
//double values[1000];
float value;
float offset1 = 3.3 / 4096;
float offset2 = 3.3 / 8192;
float gain = 11.0;
unsigned int duty = 0;

void setup() {
  Serial.begin(115200);

  ADC->ADC_MR |= 0x80;  //set free running mode on ADC and 12-bit resolution | pdf page 1345 0x480 = 3u  
  ADC->ADC_CHDR = 0xFFFF; //Disable all channel to be shore | pdf page 1351
  ADC->ADC_CHER = 0xF0; //enable ADC on pin A0 | pdf page 1350 | all 3CFF
}

long getmedian(int a, int b, int c) {
  return max(min(a, b), min(max(a, b), c));
}

long getmedian2(int a, int b, int c) {
  return a ^ b ^ c ^ (max(max(a, b), c)) ^ (min(min(a, b), c));
}

long getmedian3(int a, int b, int c, int d) {
  return ((a + b + c + d) - min(min(a, b), min(c, d)) - max(max(a, b), max(c, d))) / 2.0;
}

void loop() {
  double values[1000];
  unsigned int i = 0;
  start_time = micros();
  for (i; i < 1000; i++) {
    while ((ADC->ADC_ISR & 0xF0) == 0);// wait for conversion
    //values[i] = getmedian3(ADC->ADC_CDR[7], ADC->ADC_CDR[6], ADC->ADC_CDR[5], ADC->ADC_CDR[4]) * offset1 * gain;
    values[i] = ((ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4]) >> 2) * offset1 * gain;

    while ((ADC->ADC_ISR & 0xF0) == 0);// wait for conversion
    //values[i] += getmedian3(ADC->ADC_CDR[7], ADC->ADC_CDR[6], ADC->ADC_CDR[5], ADC->ADC_CDR[4]) * offset1 * gain;
    values[i] += ((ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4]) >> 2) * offset1 * gain;// * offset * gain;
    values[i] = round(values[i] * 1000.0) / 2000.0;
  }
//    stop_time = micros();
//    Serial.print("Total time: ");
//    Serial.println(stop_time - start_time);
//    Serial.print("Average time per conversion: ");
//    Serial.println((float)(stop_time - start_time) / 1000);
//    Serial.println(values[999], 3);

  for (int i = 0; i < 1000; i++) {
    Serial.println(values[i]*1000);
  }
  delay(1000);
  //exit(0);
}
