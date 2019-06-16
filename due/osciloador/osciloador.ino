#define PERIOD 1680
#define Vdef 2900

#define usph 3600000000
#define dist 0.0003

unsigned long start_time;
unsigned long stop_time;
float tempo;

void setup () {
  Serial.begin(115200);

  // PWM configuration
  REG_PMC_PCER1 |= PMC_PCER1_PID36;                    // Enable PWM
  REG_PIOC_ABSR |= PIO_ABSR_P24 | PIO_ABSR_P21;        // Set the port C PWM pins to peripheral type B (PWM7 physical port is PWM6) | (PWM4 physical port is PWM9)
  REG_PIOC_PDR |= PIO_PDR_P24  | PIO_PDR_P21;          // Set the port C PWM pins to outputs           (PWM7 physical port is PWM6) | (PWM4 physical port is PWM9)
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);     // Set the PWM clock A rate to 84MHz (84MHz/1)

  PWM->PWM_CH_NUM[7].PWM_CMR =  PWM_CMR_CPRE_CLKA;     // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[7].PWM_CPRD = PERIOD;                // Set the PWM period register 84MHz/(PERIOD)= Freq
  PWM->PWM_CH_NUM[7].PWM_CDTY = PERIOD / 2;            // Duty recommended starting at 0 or (Vref/Vin) * PERIOD as the Vin is variable not using here

  PWM->PWM_CH_NUM[4].PWM_CMR =  PWM_CMR_CPRE_CLKA;     // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[4].PWM_CPRD = PERIOD;                // Set the PWM period register 84MHz/(PERIOD)= Freq
  PWM->PWM_CH_NUM[4].PWM_CDTY = PERIOD / 2;            // Duty recommended starting at 0 or (Vref/Vin) * PERIOD as the Vin is variable not using here

  REG_PWM_ENA = PWM_ENA_CHID7 | PWM_ENA_CHID4; // Enable PWM CH 7 e 4

  // ADC configuration
  PMC->PMC_PCER1 |= PMC_PCER1_PID37;   // ADC power on
  ADC->ADC_CR = ADC_CR_SWRST;          // Reset ADC
  ADC->ADC_MR |=  ADC_MR_FREERUN;      // Enable free run

  ADC->ADC_CHDR = 0xFFFF; //Disable all adc channels
  ADC->ADC_CHER |= ADC_CHER_CH0 | ADC_CHER_CH7; // Enable Channels 0 and 7 physical port A0 and A7

  //LCD configuration
  PIOC->PIO_PER |= PIO_PC6;
  PIOC->PIO_OER |= PIO_PC6;
  delay(1000);
  ledoff();
}

void ledon() {
  PIOC->PIO_SODR |= PIO_PC6;//pin 23 LCD brigthness
}

void ledoff() {
  PIOC->PIO_CODR |= PIO_PC6;//pin 23 LCD brigthness
}

static int v0;
static int v7;
static int count1;
static int count2;

static bool check1 = true;
static bool check2 = true;

void loop() {

  while ((ADC->ADC_ISR & 0xFFFF) == 0);// wait for conversion

  if (check1) {
    if (ADC->ADC_CDR[0] < Vdef) {
      start_time = micros();
      ledon();
      check1 = false;
    }
  } else {
    if (ADC->ADC_CDR[7] < Vdef) {
      stop_time =  micros();
      ledoff();
      check1 = true;

      tempo = (float)(stop_time - start_time) / usph;
      Serial.print(" Vel ");
      Serial.print((float)(dist / tempo), 2);
      Serial.println(" Km/h.");
    }
  }

//  Serial.print("v ");
//  Serial.println(ADC->ADC_CDR[7]);
//
//  delay(1000);
}
