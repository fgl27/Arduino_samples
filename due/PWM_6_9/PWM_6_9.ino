
int PERIOD = 420; // 84MHz/420 = 200Khz
int DUTY = 0; // start at 50%

// Time Testing var
int loopsize = 1000;

// hw pin 6 is PWM_CH_NUM[7], hw pin 9 is PWM_CH_NUM[4]
void setup() {
  Serial.begin(9600);

  REG_PMC_PCER1 |= PMC_PCER1_PID36;                                               // Enable PWM
  REG_PIOC_ABSR |= PIO_ABSR_P24 | PIO_ABSR_P21;     // Set the port C PWM pins to peripheral type B
  REG_PIOC_PDR |= PIO_PDR_P24 | PIO_PDR_P21;          // Set the port C PWM pins to outputs
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);                                // Set the PWM clock A rate to 84MHz (84MHz/1)

  PWM->PWM_CH_NUM[4].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[4].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[4].PWM_CDTY = DUTY;

  PWM->PWM_CH_NUM[7].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[7].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[7].PWM_CDTY = DUTY;

  REG_PWM_ENA = PWM_ENA_CHID7 | PWM_ENA_CHID4 ;

}

void loop() {
  Serial.println("start");
  for (int i = 0; i < loopsize; i++) {
    DUTY++;
    if (DUTY > PERIOD) DUTY = 0;
    PWM->PWM_CH_NUM[4].PWM_CDTYUPD = DUTY; //Register to Update the duty | page 1061
    PWM->PWM_CH_NUM[7].PWM_CDTYUPD = DUTY; //Register to Update the duty | page 1061
    while (PWM->PWM_CH_NUM[4].PWM_CDTY != DUTY); // wait for a duty change
    while (PWM->PWM_CH_NUM[7].PWM_CDTY != DUTY); // wait for a duty change
    delay(10);
  }
  Serial.println("end");
  
  delay(1000);//To finish print
}
