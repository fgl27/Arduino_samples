#include <stdio.h>
#include <math.h>

#define PERIOD 280 // 84MHz/420 = 200Khz // 84MHz/336 = 250Khz // 84MHz/280 = 300Khz // 84MHz/240 = 350Khz
#define OFFSET (3.3 / 4096.0)
#define GAIN 10.985
#define MULTIPLIER (OFFSET * GAIN)

#define M 1.60594968730426
#define C 9.60691658374504

#define OFFSET_MINUS -0.6368 //-0.636812441329719
#define OFFSET_MULTI  0.1415 //0.134917890112229
#define OFFSET_MULTI2 0.115//0.134917890112229
#define OFFSET_MULTI3 0.1475

#define Vref 4 // the vout that we wanna

static uint32_t DUTY = 0; // start at 0%

void setup () {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  // ADC configuration

  PMC->PMC_PCER1 |= PMC_PCER1_PID37;      // ADC power on
  ADC->ADC_CR = ADC_CR_SWRST;            // Reset ADC

  ADC->ADC_MR |=  ADC_MR_FREERUN;

  ADC->ADC_ACR = ADC_ACR_IBCTL(0x1); // Only if 1 MHz > conversion frequency > 500 KHz

  ADC->ADC_CHDR = 0xFFFF; //Disable all channel to be shore | pdf page 1351

  //  adcgain(A2);
  //  adcgain(A3);
  //  adcgain(A4);
  //  adcgain(A5);
  //  adcgain(A6);
  //  adcgain(A7);

  ADC->ADC_CHER |=  0xFC; // Enable Channels 2 a 7

  //PWM configuration pin 6 e 9
  REG_PMC_PCER1 |= PMC_PCER1_PID36;                                               // Enable PWM
  REG_PIOC_ABSR |= PIO_ABSR_P24 | PIO_ABSR_P23 | PIO_ABSR_P22 | PIO_ABSR_P21;     // Set the port C PWM pins to peripheral type B
  REG_PIOC_PDR |= PIO_PDR_P24 | PIO_PDR_P23 | PIO_PDR_P22 | PIO_PDR_P21;          // Set the port C PWM pins to outputs
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);                                // Set the PWM clock A rate to 84MHz (84MHz/1)

  PWM->PWM_CH_NUM[4].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[4].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[4].PWM_CDTY = DUTY;

  PWM->PWM_CH_NUM[7].PWM_CMR =  PWM_CMR_CPRE_CLKA;                  // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[7].PWM_CPRD = PERIOD;                               // Set the PWM period register 84MHz/(40kHz)=2100;
  PWM->PWM_CH_NUM[7].PWM_CDTY = DUTY;

  //PWM->PWM_IER1 = PWM_IER1_CHID7;                      // Interrupt on end of counter period (CPRD)
  //NVIC_EnableIRQ(PWM_IRQn);                            // Enable PWM interrupt

  while ((ADC->ADC_ISR & 0xFC) == 0);// wait for conversion before starting the PWM, this way when PWM_Handler first run the adc will be alredy working

  REG_PWM_ENA = PWM_ENA_CHID7; //| PWM_ENA_CHID6 | PWM_ENA_CHID5 |
}

float mvolts(float val) {
  val *= MULTIPLIER;
  val += (val - 4.715) * 0.145;
  return val;
}

void adcgain(const byte aPin) {
  adc_channel_num_t ch = (adc_channel_num_t)(g_APinDescription[aPin].ulADCChannelNumber);
  adc_enable_anch(ADC);
  adc_set_channel_input_gain(ADC, ch, ADC_GAINVALUE_2);
}

void loop() {
  static float Vdif;
  static float Vout = 461;
  static unsigned long start_time;
  static unsigned long stop_time;

  start_time = micros();
  for (int i = 0; i < 1000; i++) {
    Vdif = (5.7529830359305 * pow(10, -26) *  pow(Vout, 10)) -
           (2.5043178917573 *  pow(10, -22) * pow(Vout, 9)) +
           (4.613875631129 * pow(10, -19) * pow(Vout, 8)) -
           (4.6709993937555 * pow(10, -16) * pow(Vout, 7)) +
           (0.00000000000028217490277618 * pow(Vout, 6)) -
           (0.00000000010351416307477 * pow(Vout, 5)) +
           (0.0000000227637725080466 * pow(Vout, 4)) -
           (0.00000313053515298442 * pow(Vout, 3)) +
           (0.000409991307922 * pow(Vout, 2)) -
           (0.071242586023397 * Vout) +
           (7.60152381446243);
           Vout++;
  }
  stop_time = micros();
  Serial.print("Total time: ");
  Serial.println(stop_time - start_time);
  Serial.print("Average time per conversion: ");
  Serial.println((float)(stop_time - start_time) / 1000);
  
  Serial.println(Vdif, 10);

  Serial.println(pow(2, 8), 10);
  Serial.println(2 * 2 * 8, 10);

  delay(1000);
  exit(0);
}

void PWM_Handler() {
  static float Vout;
  static float Vdif;
  static float uk = 0, uk_1 = 0, uk_2 = 0, xk = 0, xk_1 = 0, xk_2 = 0;

  PWM->PWM_ISR1;       // Clear status register

  //Reads and convert adc values to a voltage reading
  Vout = (ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4] + ADC->ADC_CDR[3] + ADC->ADC_CDR[2]) / 6;
  if (Vout < 847) {
    //Vdif = (5.7529830359305 * pow(10, -26) *  pow(Vout, 10)) - (2.5043178917573 *  pow(10, -22) * pow(Vout, 9)) + (4.613875631129 * pow(10, -19) * pow(Vout, 8)) - (4.6709993937555 * pow(10, -16)* pow(Vout, 7)) + (0.00000000000028217490277618 * pow(Vout, 6)) - (0.00000000010351416307477 * pow(Vout, 5)) + (0.0000000227637725080466 * pow(Vout, 4)) - (0.00000313053515298442 * pow(Vout, 3) + 0.000409991307922 * pow(Vout, 2)) - (0.071242586023397 * Vout) + (7.60152381446243);
    Vdif = 0.225;
  } else Vdif = 0;
  Vout *= (OFFSET * (GAIN - Vdif));


  //Vout = (ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4] + ADC->ADC_CDR[3] + ADC->ADC_CDR[2]) / 6 * OFFSET;
  //Vout *= (Vout*M + C);

  //Vout = (ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4] + ADC->ADC_CDR[3] + ADC->ADC_CDR[2]) / 6 * MULTIPLIER;
  //Vout += (Vout*(Vout >= 4.72 ? OFFSET_MULTI : OFFSET_MULTI2) + OFFSET_MINUS);

  //Vout = (ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4] + ADC->ADC_CDR[3] + ADC->ADC_CDR[2]) / 6 * MULTIPLIER;
  //Vout += (Vout - 4.72) * (OFFSET_MULTI3 - (0.00085 * Vout));

  xk = Vref - Vout;

  uk =  44.8983097883 * xk + 2.7027784604 * xk_1 - 42.1955313279 * xk_2 + 0.4714280152 * uk_1 + 0.5285719848 * uk_2; //tustin

  DUTY = uk * PERIOD / 8;

  if (DUTY > PERIOD)   DUTY = PERIOD;
  if (DUTY < 0)        DUTY = 0;

  xk_2 = xk_1;
  xk_1 = xk;
  uk_2 = uk_1;
  uk_1 = uk;
  PWM->PWM_CH_NUM[7].PWM_CDTYUPD = DUTY;
}
