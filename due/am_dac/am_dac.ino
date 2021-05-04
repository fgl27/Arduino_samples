/****************************************************************************************************/
/*  0.7 MHz ADC conversions of 1 analog input (A0) triggered by Timer Counter 0 channel 2 TIOA2       */
/*  1.4 MHz DAC output on channel 1 (DAC1) triggered by Timer Counter 0 channel 2 TIOA2               */
/****************************************************************************************************/
#define FREQ 700 // Operation Freq, not exactly math some freq will be close but not waht write here, Max freq 807.44 kHz max dac freq
//sudo chmod a+rw /dev/ttyACM0 

void setup()
{

  adc_setup();
  dac_setup();
  tc_setup();
}

void loop()
{

}

/*************  Configure adc_setup function  *******************/
void adc_setup() {

  PMC->PMC_PCER1 |= PMC_PCER1_PID37;                    // ADC power ON

  ADC->ADC_CR = ADC_CR_SWRST;                           // Reset ADC
  ADC->ADC_MR |=  ADC_MR_TRGEN_EN                       // Hardware trigger select
                  | ADC_MR_TRGSEL_ADC_TRIG3             // Trigger by TIOA2
                  | ADC_MR_PRESCAL(1);

  ADC->ADC_ACR = ADC_ACR_IBCTL(0b01);                   // For frequencies > 500 KHz

  ADC->ADC_IER = ADC_IER_EOC7;                          // End Of Conversion interrupt enable for channel 7
  NVIC_EnableIRQ(ADC_IRQn);                                  // Enable ADC interrupt
  ADC->ADC_CHER = ADC_CHER_CH7;                         // Enable Channel 7 = A0
}

void ADC_Handler () {
  static bool enable = true;
  /* Todo : Apply any digital filtering before DAC output  */
  /* Beware : Stay in ADC_Handler much less than 1 us  !!! */
  DACC->DACC_CDR = enable ? ADC->ADC_CDR[7] : 0;                    // Reading ADC->ADC_CDR[i] clears EOCi bit
  enable = !enable;

}

/*************  Configure dacc_setup function  *******************/
void dac_setup ()
{

  PMC->PMC_PCER1 = PMC_PCER1_PID38;                   // DACC power ON

  DACC->DACC_CR = DACC_CR_SWRST ;                     // Reset DACC
  DACC->DACC_MR = DACC_MR_TRGEN_EN                    // Hardware trigger select
                  | DACC_MR_TRGSEL(0b011)             // Trigger by TIOA2
                  | DACC_MR_USER_SEL_CHANNEL1         // select channel 1
                  | DACC_MR_REFRESH (1)
                  | DACC_MR_STARTUP_8
                  | DACC_MR_MAXS;

  DACC->DACC_CHER = DACC_CHER_CH1;                   // enable channel 1 = DAC1

}

/*************  Timer Counter 0 Channel 2 to generate PWM pulses thru TIOA2  ************/
void tc_setup() {

  PMC->PMC_PCER0 |= PMC_PCER0_PID29;                      // TC2 power ON : Timer Counter 0 channel 2 IS TC2

  TC0->TC_CHANNEL[2].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1  // MCK/2, clk on rising edge
                              | TC_CMR_WAVE               // Waveform mode
                              | TC_CMR_WAVSEL_UP_RC       // UP mode with automatic trigger on RC Compare
                              | TC_CMR_ACPA_CLEAR         // Clear TIOA2 on RA compare match
                              | TC_CMR_ACPC_SET;          // Set TIOA2 on RC compare match

  float freq_rc = 42000 / (FREQ * 2);
  TC0->TC_CHANNEL[2].TC_RC = freq_rc;  //<*********************  Frequency = (Mck/2)/TC_RC  Hz = 1.4 MHz
  TC0->TC_CHANNEL[2].TC_RA = freq_rc / 2;  //<********************   Any Duty cycle in between 1 and TC_RC

  TC0->TC_CHANNEL[2].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN;// Software trigger TC2 counter and enable

}
