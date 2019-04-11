// Enable single slope, 11-bit resolution PWM at 40kHz on 8 channels
void setup() {
  // PWM set-up on pins DAC1, A8, A9, A10, D9, D8, D7 and D6 for channels 0 through to 7 respectively
  REG_PMC_PCER1 |= PMC_PCER1_PID36;                                               // Enable PWM 
  //REG_PIOB_ABSR |= PIO_ABSR_P19 | PIO_ABSR_P18 | PIO_ABSR_P17 | PIO_ABSR_P16;     // Set the port B PWM pins to peripheral type B
  REG_PIOC_ABSR |= PIO_ABSR_P24 | PIO_ABSR_P23 | PIO_ABSR_P22 | PIO_ABSR_P21;     // Set the port C PWM pins to peripheral type B
  //REG_PIOB_PDR |= PIO_PDR_P19 | PIO_PDR_P18 | PIO_PDR_P17 | PIO_PDR_P16;          // Set the port B PWM pins to outputs
  REG_PIOC_PDR |= PIO_PDR_P24 | PIO_PDR_P23 | PIO_PDR_P22 | PIO_PDR_P21;          // Set the port C PWM pins to outputs
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);                                // Set the PWM clock A rate to 84MHz (84MHz/1)
  //REG_PWM_SCM |= PWM_SCM_SYNC7 | PWM_SCM_SYNC6 | PWM_SCM_SYNC5 | PWM_SCM_SYNC4 |  // Set the PWM channels as synchronous
  //               PWM_SCM_SYNC3 | PWM_SCM_SYNC2 | PWM_SCM_SYNC1 | PWM_SCM_SYNC0;  
  for (uint8_t i = 0; i < PWMCH_NUM_NUMBER; i++)                      // Loop for each PWM channel (8 in total)
  {
    PWM->PWM_CH_NUM[i].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
    PWM->PWM_CH_NUM[i].PWM_CPRD = 420;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  } 
  //REG_PWM_ENA = PWM_ENA_CHID0;           // Enable the PWM channels, (only need to set channel 0 for synchronous mode)
  REG_PWM_ENA = PWM_ENA_CHID7 | PWM_ENA_CHID6 | PWM_ENA_CHID5 | PWM_ENA_CHID4 |    // Enable all PWM channels
                PWM_ENA_CHID3 | PWM_ENA_CHID2 | PWM_ENA_CHID1 | PWM_ENA_CHID0;
  for (uint8_t i = 0; i < PWMCH_NUM_NUMBER; i++)                      // Loop for each PWM channel (8 in total)
  {
    PWM->PWM_CH_NUM[i].PWM_CDTYUPD = 300;                            // Set the PWM duty cycle to 50% (2100/2=1050) on all channels
  } 
  //REG_PWM_SCUC = PWM_SCUC_UPDULOCK;      // Set the update unlock bit to trigger an update at the end of the next PWM period
}

void loop() {}
