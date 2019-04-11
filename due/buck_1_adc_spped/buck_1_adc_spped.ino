
void setup () {
  Serial.begin(115200);

  // ADC configuration
  PMC->PMC_PCER1 |= PMC_PCER1_PID37;   // ADC power on
  ADC->ADC_CR = ADC_CR_SWRST;          // Reset ADC
  ADC->ADC_MR |=  ADC_MR_FREERUN;      // Enable free run

  ADC->ADC_MR |= ADC_MR_PRESCAL(0);       // MCK/2 prescaler
  ADC->ADC_MR |= ADC_MR_STARTUP_SUT0;     // Startup cycles
  ADC->ADC_MR |= ADC_MR_TRACKTIM(0);     // Tracking cycles
  ADC->ADC_MR |= ADC_MR_TRANSFER(1);      // Transfer cycles
  ADC->ADC_MR |= ADC_MR_SETTLING_AST3;    // Settling cycles

  ADC->ADC_ACR = ADC_ACR_IBCTL(0b01);  // Only if 1 MHz > conversion frequency > 500 KHz

  ADC->ADC_CHDR = 0xFFFF; //Disable all adc channels
  ADC->ADC_CHER |=  ADC_CHER_CH4 |
                    ADC_CHER_CH5 | ADC_CHER_CH6 | ADC_CHER_CH7; // Enable Channels 2 to 7 physical port A0 to A5
}

void loop() {
  Serial.print("Start: ");
  static float Vout;
  static unsigned long start_time;
  static unsigned long stop_time;

  start_time = micros();
  for (int i = 0; i < 1000; i++) {
    while ((ADC->ADC_ISR & (ADC_CHER_CH4 | ADC_CHER_CH5 | ADC_CHER_CH6 | ADC_CHER_CH7)) == 0); // wait for conversion
    Vout = (ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4]) / 4;
  }
  stop_time = micros();
  Serial.print("Total time: ");
  Serial.println(stop_time - start_time);
  Serial.print("Average time per conversion: ");
  Serial.println((float)(stop_time - start_time) / 1000);
  Serial.println(Vout);

  delay(500);
  //exit(0);
}
