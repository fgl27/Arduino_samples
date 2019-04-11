
int PERIOD = 420; // 84MHz/420 = 200Khz
int DUTY = 0; // start at 50%
int PWM_PIMS[2] = {6, 7};// PIM_BUCK, PIM_BOOST

// Time Testing var
unsigned long start_time;
unsigned long stop_time;
int loopsize = 1000;

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
}


void loop() {
  start_time = micros();
  for (int i = 0; i < loopsize; i++) {
    DUTY++;
    if (DUTY > PERIOD) DUTY = 0;
    PWM->PWM_CH_NUM[PWM_PIMS[0]].PWM_CDTYUPD = DUTY; //Register to Update the duty | page 1061
    PWM->PWM_CH_NUM[PWM_PIMS[1]].PWM_CDTYUPD = DUTY; //Register to Update the duty | page 1061
    while (PWM->PWM_CH_NUM[PWM_PIMS[0]].PWM_CDTY != DUTY); // wait for a duty change
    while (PWM->PWM_CH_NUM[PWM_PIMS[1]].PWM_CDTY != DUTY); // wait for a duty change
  }
  stop_time = micros();

    PWM->PWM_CH_NUM[PWM_PIMS[0]].PWM_CDTYUPD = 350; //Register to Update the duty | page 1061
    PWM->PWM_CH_NUM[PWM_PIMS[1]].PWM_CDTYUPD = 350; //Register to Update the duty | page 1061

  Serial.print("Total time: ");
  Serial.println(stop_time - start_time);
  Serial.print("Average time per conversion: ");
  Serial.println((float)(stop_time - start_time) / loopsize);

  Serial.println(PWM->PWM_CH_NUM[PWM_PIMS[0]].PWM_CDTY);
  Serial.println(PWM->PWM_CH_NUM[PWM_PIMS[1]].PWM_CDTY);
  
  delay(1000);//To finish print
  exit(0);
}
