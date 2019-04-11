
/***********************************************************************************************/
/*                 PWMH2 and PWML2 Enabled -   PWM frequency = 200 KHz                          */
/***********************************************************************************************/

void setup () {
  pinMode(LED_BUILTIN, OUTPUT);
  PWM_Init();
}

void loop() {
}

void PWM_Init() {
  // PWM Set-up on pins PC7 and PA20 (Arduino Pins 39(PWMH2) and 43(PWML2)): see Datasheet chap. 38.5.1 page 973
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                   // PWM power ON
  PWM->PWM_DIS = PWM_DIS_CHID2;                        // Disable PWM channel 2

  // Select Instance=PWM; Signal=PWMH2 (channel 2); I/O Line=PC7 (P7, Arduino pin 39, see pinout diagram) ; Peripheral type B
  PIOC->PIO_PDR |= PIO_PDR_P7;                          // Set the pin to the peripheral PWM, not the GPIO
  PIOC->PIO_ABSR |= PIO_PC7B_PWMH2;                     // Set PWM pin perhipheral type B

  // Select Instance=PWM; Signal=PWML2 (channel 2); I/O Line=PA20 (P20, Arduino pin 43, see pinout diagram) ; Peripheral type B
  PIOA->PIO_PDR |= PIO_PDR_P20;                          // Set the pin to the peripheral PWM, not the GPIO
  PIOA->PIO_ABSR |= PIO_PA20B_PWML2;                    // Set PWM pin perhipheral type B

  PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);   // Set the PWM clock rate to 2MHz (84MHz/42). Waveform left aligned
  PWM->PWM_CH_NUM[2].PWM_CMR = PWM_CMR_CPRE_CLKA;      // The period is left aligned, clock source as CLKA on channel 2
  PWM->PWM_CH_NUM[2].PWM_CPRD = 420;                   // Channel 2 : Set the PWM frequency 2MHz/ CPRD = F ;
  PWM->PWM_CH_NUM[2].PWM_CDTY = 420 / 2;                    // Channel 2: Set the PWM duty cycle to x%= (CDTY/ CPRD)  * 100 % ;

  // Dead times
  PWM->PWM_CH_NUM[2].PWM_DT = PWM_DT_DTH(2);           // Dead Time for PWMH; 0 < DT < (CPRD - CDTY)  12 bit max
  PWM->PWM_CH_NUM[2].PWM_DT = PWM_DT_DTL(2);          // Dead Time for PWML; 0 < DT < (CPRD - CDTY)
  
  PWM->PWM_IER1 = PWM_IER1_CHID2;                      // Interrupt on end of counter period (CPRD)
  NVIC_EnableIRQ(PWM_IRQn);                            // Enable PWM interrupt

  PWM->PWM_ENA = PWM_ENA_CHID2;                        // Enable PWM Channel 2
}

void PWM_Handler() {
  static uint32_t Count;
  PWM->PWM_ISR1;      // Clear status register
  Count++;
  if (Count == 200000) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle every 1 Hz
    Count = 0;
  }
}
