
float offset = 3.3 / 4096.0;
float gain = 10.34;
float multiplier = offset * gain;

int PERIOD = 420; // 84MHz/420 = 200Khz
int DUTY = 0; // start at 0%
int AL_10 = (PERIOD / 20 ) * 9; // start at 10%
int N_10 = (PERIOD / 10 ) * 9; // start at 10%

int CLOCK = 84000000 / PERIOD;

float volts = 0;

void setup () {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  // ADC configuration

  PMC->PMC_PCER1 |= PMC_PCER1_PID37;      // ADC power on
  ADC->ADC_CR = ADC_CR_SWRST;            // Reset ADC

  ADC->ADC_MR |=  ADC_MR_FREERUN;

  ADC->ADC_ACR = ADC_ACR_IBCTL(0x1); // Only if 1 MHz > conversion frequency > 500 KHz

  ADC->ADC_CHDR = 0xFFFF; //Disable all channel to be shore | pdf page 1351
  ADC->ADC_CHER |=  0xFC; // Enable Channels 2 a 7

  //PWM configuration pin 6 e 9
  REG_PMC_PCER1 |= PMC_PCER1_PID36;                                               // Enable PWM
  REG_PIOC_ABSR |= PIO_ABSR_P24 | PIO_ABSR_P23 | PIO_ABSR_P22 | PIO_ABSR_P21;     // Set the port C PWM pins to peripheral type B
  REG_PIOC_PDR |= PIO_PDR_P24 | PIO_PDR_P23 | PIO_PDR_P22 | PIO_PDR_P21;          // Set the port C PWM pins to outputs
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);                                // Set the PWM clock A rate to 84MHz (84MHz/1)

  PWM->PWM_CH_NUM[4].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[4].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[4].PWM_CDTY = PERIOD / 2;

  PWM->PWM_CH_NUM[7].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[7].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[7].PWM_CDTY = PERIOD / 2;

  PWM->PWM_IER1 = PWM_IER1_CHID7;                      // Interrupt on end of counter period (CPRD)
  NVIC_EnableIRQ(PWM_IRQn);                            // Enable PWM interrupt
  
  REG_PWM_ENA = PWM_ENA_CHID7 | PWM_ENA_CHID4; //| PWM_ENA_CHID6 | PWM_ENA_CHID5 |
}

float mvolts(float val) {
  val *= multiplier;
  val += (val - 4.715) * 0.145;
  return val;
}

void loop() {
//  long start_time = micros();
//  for (int i = 0; i < 1000; i++) {
//    while ((ADC->ADC_ISR & 0xFC) == 0);// wait for conversion
//    volts = (float)((ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4] + ADC->ADC_CDR[3] + ADC->ADC_CDR[2]) / 6);
//    //Serial.println(volts);
//  }
//  long stop_time = micros();
//  Serial.print("Total time: ");
//  Serial.println(stop_time - start_time);
//  Serial.print("Average time per conversion: ");
//  Serial.println((float)(stop_time - start_time) / 1000);
//  Serial.println(mvolts(volts), 4);
//
//  //  DUTY++;
//  //  if (DUTY > 100) DUTY = 0;
//  //  PWM->PWM_CH_NUM[7].PWM_CDTYUPD = DUTY;
//  //PWM->PWM_CH_NUM[4].PWM_CDTYUPD = DUTY;
//  //Serial.println(DUTY);
//  //Serial.print("volts = ");
//  //Serial.println(volts);
//
//  delay(1000);
//  exit(0);
}

void PWM_Handler() {
  static uint32_t Count;
  static long start_time, stop_time;
  PWM->PWM_ISR1;      // Clear status register
  Count++;
  while ((ADC->ADC_ISR & 0xFC) == 0);// wait for conversion
  volts = (ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4] + ADC->ADC_CDR[3] + ADC->ADC_CDR[2]) / 6;
  if (Count == CLOCK) {
    start_time = micros();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle every 1 Hz
    Serial.print("Periodo: ");
    Serial.print((start_time - stop_time) / CLOCK);
    Serial.print(" Micros, Frequencia: ");
    Serial.print(CLOCK);
    Serial.println(" Hz");
    stop_time = start_time;
    Count = 0;
  }
}
