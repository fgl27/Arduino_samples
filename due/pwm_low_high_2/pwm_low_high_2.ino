
/***********************************************************************************************/
/*                 PWMH0 and PWML0 Enabled -   PWM frequency = 200 KHz                          */
/***********************************************************************************************/

#define PERIOD 420 // 84MHz/420 = 200Khz // 84MHz/336 = 250Khz // 84MHz/280 = 300Khz // 84MHz/240 = 350Khz
#define DEADTIME 3

void setup () {
  pinMode(LED_BUILTIN, OUTPUT);
  PWM_Init();
}

void loop() {
}

void PWM_Init() {
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

  PWM->PWM_ENA = PWM_ENA_CHID0;                        // Enable PWM Channel 2
}
void PWM_Handler() {
  static uint32_t Count;
  static int DUTY;
  PWM->PWM_ISR1;      // Clear status registerDEADTIME
  Count++;
  if (Count == 20000) {
    //DUTY++;
    //if (DUTY > PERIOD) DUTY = 0;
    //PWM->PWM_CH_NUM[0].PWM_CDTYUPD = DUTY;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Count = 0;
  }
}
