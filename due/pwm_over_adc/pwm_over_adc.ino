int PERIOD = 420; // 84MHz/420 = 200Khz
int DUTY = 0; // start at 50%
int PWM_PIMS[2] = {6, 7};// PIM_BUCK, PIM_BOOST

// Time Testing var
unsigned long start_time;
unsigned long stop_time;
int loopsize = 1000;

unsigned long values[1000];
int i;

void setup () {
  Serial.begin(115200);
  int32_t PWM_PIM;

  //Enabling PWM and base clock on it
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                  // Enable PWM
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);    // Set the PWM clock rate to 84MHz (84MHz/1)

  //Specific PIN configuration for it pim in PWM_PIMS
  for (int i = 0; i < sizeof(PWM_PIMS) / sizeof(int); i++) {
    PWM_PIM = digitalPinToBitMask(PWM_PIMS[i]); // Convert pim number to mask
    REG_PIOC_ABSR |= PWM_PIM;                   // Set PWM pins perhipheral to type B as PWM pim (Table 39-2. I/O Lines pag 986)
    REG_PIOC_PDR |= PWM_PIM;                    // Set PWM pins to an output

    // PWM_CMR The period must be is center aligned CALG = 0, to be able to check PWM_CDTY != DUTY as sone as it changes
    PWM->PWM_CH_NUM[PWM_PIMS[i]].PWM_CMR = PWM_CMR_CPRE_CLKA;  // Set clock source as CLKA
    PWM->PWM_CH_NUM[PWM_PIMS[i]].PWM_CPRD = PERIOD;            // Set the PWM frequency 84MHz/420 = 200 kHz
    PWM->PWM_CH_NUM[PWM_PIMS[i]].PWM_CDTY = DUTY;              // Set the PWM duty cycle 50% (PERIOD / 2)
    REG_PWM_ENA = 1 << PWM_PIMS[i];             // Enable the PWM channels
  }

  ADC->ADC_MR |= 0x80;  //set free running mode on ADC and 12-bit resolution | pdf page 1345
  ADC->ADC_CHDR = 0xFFFFFFFF; //Disable all channel to be shore | pdf page 1351
  ADC->ADC_CHER = 0x80; //enable ADC on pin A0 | pdf page 1350
}


void loop() {
  start_time = micros();
  for (i = 0; i < loopsize; i++) {
    DUTY++;
    if (DUTY > PERIOD) DUTY = 0;
    PWM->PWM_CH_NUM[PWM_PIMS[0]].PWM_CDTYUPD = DUTY; //Register to Update the duty | page 1061
    while (PWM->PWM_CH_NUM[PWM_PIMS[0]].PWM_CDTY != DUTY); // wait for a duty change
    //values[i] = (ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5]) / 3;
    values[i] = ADC->ADC_CDR[7];
    while ((ADC->ADC_ISR & 0x80) == 0);
    values[i] = (values[i] + ADC->ADC_CDR[7]) / 2;
  }

  Serial.println("Values: ");
  for (i = 0; i < 1000; i++) {
    Serial.println(values[i]);
    delay(10);
  }
}
