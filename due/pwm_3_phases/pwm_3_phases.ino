
float offset = 3.3 / 4095.0;
float offsetdiff = 3.3 / 2047.0;
float gain = 1;

int PERIOD = 420; // 84MHz/420 = 200Khz
int DUTY = 0; // start at 0%
int AL_10 = (PERIOD / 20 ) * 9; // start at 10%
int N_10 = (PERIOD / 10 ) * 9; // start at 10%

float volts = 0;

void setup () {
  Serial.begin(115200);

  //PWM configuration pin 6 e 9
  REG_PMC_PCER1 |= PMC_PCER1_PID36;                                               // Enable PWM
  REG_PIOC_ABSR |= PIO_ABSR_P24 | PIO_ABSR_P23 | PIO_ABSR_P22 | PIO_ABSR_P21;     // Set the port C PWM pins to peripheral type B
  REG_PIOC_PDR |= PIO_PDR_P24 | PIO_PDR_P23 | PIO_PDR_P22 | PIO_PDR_P21;          // Set the port C PWM pins to outputs
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);                                // Set the PWM clock A rate to 84MHz (84MHz/1)

  PWM->PWM_CH_NUM[7].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[7].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[7].PWM_CDTY = 0;

  PWM->PWM_CH_NUM[5].PWM_CMR =  PWM_CMR_CPRE_CLKA | PWM_CMR_CALG | PWM_CMR_CPOL; //| PWM_CMR_CALG | PWM_CMR_CPOL
  PWM->PWM_CH_NUM[5].PWM_CPRD = PERIOD / 2;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[5].PWM_CDTY = 0;

  PWM->PWM_CH_NUM[4].PWM_CMR =  PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL; //| PWM_CMR_CALG | PWM_CMR_CPOL
  PWM->PWM_CH_NUM[4].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[4].PWM_CDTY = 0;

  REG_PWM_ENA = PWM_ENA_CHID7 | PWM_ENA_CHID5 | PWM_ENA_CHID4; //| PWM_ENA_CHID6 | PWM_ENA_CHID5 |
}

void loop() {
  DUTY++;
  if (DUTY > (PERIOD / 3)) DUTY = 0;
  PWM->PWM_CH_NUM[7].PWM_CDTYUPD = DUTY;
  PWM->PWM_CH_NUM[4].PWM_CDTYUPD = PERIOD - DUTY;
  PWM->PWM_CH_NUM[5].PWM_CDTYUPD = (PERIOD - DUTY) / 2;
  //Serial.println(DUTY);
  //Serial.print("volts = ");
  //Serial.println(volts);

  delay(100);
  //exit(0);
}
