#define PERIOD 420 // 84MHz/420 = 200Khz // 84MHz/336 = 250Khz // 84MHz/280 = 300Khz // 84MHz/240 = 350Khz
#define DEADTIME 3
#define Vref 3 // the target Vout of the sistem

#define OFFSET 3.3 / 4096 // ADC is 3.3V base with resolution of 0 to 4095 bits

// This is a approximation function for the gain, will give a + or - 0.045 accuracy for the output voltage
#define GAIN 6
//#define GAIN Vref / ((213.92819 * Vref + 19.66999) * OFFSET)
#define MULTIPLIER OFFSET
//#define MULTIPLIER (OFFSET * GAIN) / 6

//#define MULTIPLIER_TEST (OFFSET * GAIN)
//#define CLOCK (84000000 / PERIOD)

void setup () {
  //Serial.begin(115200);

  // ADC configuration
  PMC->PMC_PCER1 |= PMC_PCER1_PID37;   // ADC power on
  ADC->ADC_CR = ADC_CR_SWRST;          // Reset ADC
  ADC->ADC_MR |=  ADC_MR_FREERUN;      // Enable free run
  ADC->ADC_ACR = ADC_ACR_IBCTL(0b01);  // Only if 1 MHz > conversion frequency > 500 KHz

  ADC->ADC_CHDR = 0xFFFF; //Disable all adc channels
  ADC->ADC_CHER |=  ADC_CHER_CH2 | ADC_CHER_CH3 | ADC_CHER_CH4 |
                    ADC_CHER_CH5 | ADC_CHER_CH6 | ADC_CHER_CH7; // Enable Channels 2 to 7 physical port A0 to A5

  // PWM Set-up on pins PC7 and PA20 (Arduino Pins 39(PWMH2) and 43(PWML2)): see Datasheet chap. 38.5.1 page 973
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                   // PWM power ON
  PWM->PWM_DIS = PWM_DIS_CHID0;                        // Disable PWM channel 0

  // Select Instance=PWM; Signal=PWMH0 (channel 0); I/O Line=PC3 (P3, Arduino pin 34, see pinout diagram) ; Peripheral type B
  PIOC->PIO_PDR |= PIO_PDR_P3;                          // Set the pin to the peripheral PWM, not the GPIO
  PIOC->PIO_ABSR |= PIO_PC3B_PWMH0;                     // Set PWM pin perhipheral type B

  // Select Instance=PWM; Signal=PWML0 (channel 0); I/O Line=PC2 (P2, Arduino pin 35, see pinout diagram) ; Peripheral type B
  PIOC->PIO_PDR |= PIO_PDR_P2;                          // Set the pin to the peripheral PWM, not the GPIO
  PIOC->PIO_ABSR |= PIO_PC2B_PWML0;                    // Set PWM pin perhipheral type B

  PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);            // Set the PWM clock rate to 84MHz (84MHz/1). Waveform left aligned
  PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKA               // clock source as CLKA
                               | PWM_CMR_DTE;                  // Enable Dead time
  PWM->PWM_CH_NUM[0].PWM_CPRD = PERIOD;                        // Channel 2 : Set the PWM frequency 2MHz/ CPRD = F ;
  PWM->PWM_CH_NUM[0].PWM_CDTY = 0;                             // Channel 2: Set the PWM duty cycle to x%= (CDTY/ CPRD)  * 100 % ;

  // Dead times
  PWM->PWM_CH_NUM[0].PWM_DT |= PWM_DT_DTH(DEADTIME);           // Dead Time for PWMH; 0 < DT < (CPRD - CDTY)  12 bit max
  PWM->PWM_CH_NUM[0].PWM_DT |= PWM_DT_DTL(DEADTIME);          // Dead Time for PWML; 0 < DT < (CPRD - CDTY)

  PWM->PWM_IER1 = PWM_IER1_CHID0;                      // Interrupt on end of counter period (CPRD)
  NVIC_EnableIRQ(PWM_IRQn);                            // Enable PWM interrupt

  PWM->PWM_ENA = PWM_ENA_CHID0;                        // Enable PWM Channel 0
}

void loop() {}

void PWM_Handler() {
  //static uint32_t Count;
  //static float Bits;

  static float Vout, uk, uk_1, uk_2, xk, xk_1, xk_2;
  static int DUTY;

  PWM->PWM_ISR1; // Clear PWM status register

  //Reads and convert adc values to a voltage reading
  Vout = (ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4] + ADC->ADC_CDR[3] + ADC->ADC_CDR[2]) * MULTIPLIER;

  //For testing
  //Bits = (ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4] + ADC->ADC_CDR[3] + ADC->ADC_CDR[2]) / 6;
  //Vout = Bits * MULTIPLIER_TEST;

  //Type II Compensator, a feedback duty calculator for DC-DC converter, using the bilinear (Tustin) discretization method to generate the time function of the PID transfer function
  xk = Vref - Vout; // Error

  // Bilinear (Tustin) discretization method of the PID function of this buck
  uk =  44.8983097883 * xk + 2.7027784604 * xk_1 - 42.1955313279 * xk_2 + 0.4714280152 * uk_1 + 0.5285719848 * uk_2;

  DUTY = uk * PERIOD / 8;

  if (DUTY > PERIOD) DUTY = 200; //Prevent DUTY bigger then PERIOD
  else if (DUTY < 0) DUTY = 0; //Prevent DUTY lower then Zero

  PWM->PWM_CH_NUM[0].PWM_CDTYUPD = DUTY; //Set the DUTY

  //Set the previous values
  xk_2 = xk_1; //xk(t - 2)
  xk_1 = xk;   //xk(t - 1)
  uk_2 = uk_1; //uk(t - 2)
  uk_1 = uk;   //uk(t - 1)

  // To print testing values
  //  Count++;
  //  if (Count == CLOCK) {
  //    Serial.print(" Vout: ");
  //    Serial.println(Vout);
  //    Count = 0;
  //  }
}
