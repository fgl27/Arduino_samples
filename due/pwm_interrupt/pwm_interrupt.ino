
float offset = 3.3 / 4096.0;
float gain = 10.34;
float multiplier = offset * gain;

int PERIOD = 40000000; // 84MHz/420 = 200Khz
int DUTY = 0; // start at 0%
int count = 0;

float volts = 0;

void setup () {
  Serial.begin(115200);

  // ADC configuration

  PMC->PMC_PCER1 |= PMC_PCER1_PID37;      // ADC power on
  ADC->ADC_CR = ADC_CR_SWRST;            // Reset ADC

  //ADC->ADC_MR |=  ADC_MR_FREERUN;

  ADC->ADC_ACR = ADC_ACR_IBCTL(0x1); // Only if 1 MHz > conversion frequency > 500 KHz

  ADC->ADC_CHDR = 0xFFFF; //Disable all channel to be shore | pdf page 1351
  ADC->ADC_CHER |=  0xFC; // Enable Channels 2 a 7
  /* Enable Data ready interrupt. */
  adc_enable_interrupt(ADC, ADC_IER_DRDY);

  /* Enable ADC interrupt. */
  NVIC_EnableIRQ(ADC_IRQn);

  //PWM configuration pin 6 e 9
  REG_PMC_PCER1 |= PMC_PCER1_PID36;                                               // Enable PWM
  REG_PIOC_ABSR |= PIO_ABSR_P24 | PIO_ABSR_P23 | PIO_ABSR_P22 | PIO_ABSR_P21;     // Set the port C PWM pins to peripheral type B
  REG_PIOC_PDR |= PIO_PDR_P24 | PIO_PDR_P23 | PIO_PDR_P22 | PIO_PDR_P21;          // Set the port C PWM pins to outputs
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);                                // Set the PWM clock A rate to 84MHz (84MHz/1)

  PWM->PWM_CH_NUM[4].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[4].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[4].PWM_CDTY = PERIOD / 3.5;

  PWM->PWM_CH_NUM[7].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[7].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[7].PWM_CDTY = PERIOD / 3.5;

  REG_PWM_ENA = PWM_ENA_CHID7 | PWM_ENA_CHID4; //| PWM_ENA_CHID6 | PWM_ENA_CHID5 |

  adc_configure_trigger(ADC, ADC_TRIG_PWM_EVENT_LINE_0, 7);
}

float mvolts(float val) {
  val *= multiplier;
  val += (val - 4.715) * 0.145;
  return val;
}

void loop() {

}

uint32_t ul_temp;

void ADC_Handler(void)
{
  if (count < 1000) {
    if ((adc_get_status(ADC) & ADC_ISR_DRDY) == ADC_ISR_DRDY) {
      count++;
      ul_temp = adc_get_latest_value(ADC);
    }
  } else {
    REG_PWM_DIS = PWM_ENA_CHID7 | PWM_ENA_CHID4;
    // The serial prints are only here for debugging.
    Serial.println(count);
    Serial.println(ul_temp);
  }
}
