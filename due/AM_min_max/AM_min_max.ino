#include <stdio.h>
#include <math.h>

#define PWM_PIN 4 // 
#define ADC_PIN 7 // 
#define PERIOD 150 // 84 Mhz / 1680 = 50 Khz

//#define DUTY PERIODO / 2 // 50% duty

void setup () {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Configuração ADC, dois canais no modo de leitura livre
  PMC->PMC_PCER1 |= PMC_PCER1_PID37;   // ADC power on
  ADC->ADC_CR = ADC_CR_SWRST;          // Reset ADC
  ADC->ADC_MR |=  ADC_MR_FREERUN;      // Enable free run
  ADC->ADC_ACR = ADC_ACR_IBCTL(0b01);  // Only if 1 MHz > conversion frequency > 500 KHz

  ADC->ADC_CHDR = 0xFFFF; //Disable all adc channels
  ADC->ADC_CHER |= ADC_CHER_CH7; // Enable Channels 0 physical port A0

  //PWM configuration pin 9
  REG_PMC_PCER1 |= PMC_PCER1_PID36;                    // Enable PWM
  REG_PIOC_ABSR |= PIO_ABSR_P24 | PIO_ABSR_P21;        // Set the port C PWM pins to peripheral type B (PWM7 physical port is PWM6) | (PWM4 physical port is PWM9)
  REG_PIOC_PDR |= PIO_PDR_P24  | PIO_PDR_P21;          // Set the port C PWM pins to outputs           (PWM7 physical port is PWM6) | (PWM4 physical port is PWM9)
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);     // Set the PWM clock A rate to 84MHz (84MHz/1)

  PWM->PWM_CH_NUM[PWM_PIN].PWM_CMR =  PWM_CMR_CPRE_CLKA; // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[PWM_PIN].PWM_CPRD = PERIOD;            // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[PWM_PIN].PWM_CDTY = PERIOD / 2;                 // Duty recommended starting at 0 or (Vref/Vin) * PERIOD as the Vin is variable not using here

  PWM->PWM_IER1 = PWM_IER1_CHID4;                      // Interrupt on end of counter period (CPRD)
  NVIC_EnableIRQ(PWM_IRQn);                            // Enable PWM interrupt

  REG_PWM_ENA = PWM_ENA_CHID4; // Enable PWM CH 7 e 4

}

void loop() {

}

void PWM_Handler() {

  static char strbuf[255];
  static uint32_t Count;
  static float minv = 4096;
  static float maxv = 0;
  static float VIN = 0;
  static float value = 0;
  PWM->PWM_ISR1;      // Clear status register
  Count++;
  if (Count == 56000) {
    Count = 0;

    VIN = ADC->ADC_CDR[ADC_PIN];
    
    if (VIN < minv) minv = VIN;
    if (VIN > maxv) maxv = VIN;

    value = (maxv - minv);
    
    sprintf(
      strbuf,
      "maxv %.2f, minv %.2f, value %.2f",
      maxv,
      minv,
      value
    );
    Serial.println(strbuf);
  }

}
