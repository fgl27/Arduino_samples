#define PERIODO 1680 // 84 Mhz / 1680 = 50 Khz
#define VReferencia 2900 // Tensão referencia, se baixar desta muda o esstado do led

#define hora_em_micro 3600000000 // Uma hora em microsegundos
#define distancia 0.0003 // 30 cm convertido em Km

// Calculo de contaem do tempo
unsigned long tempo_inicial;
unsigned long tempo_final;
float tempo;

// Verifica tempo inicial? true sim, false não
static bool check_inicio = true;

void setup () {
  Serial.begin(115200);

  // Configuração do PWM, dois canais a 50Khz
  REG_PMC_PCER1 |= PMC_PCER1_PID36;                    // Enable PWM
  REG_PIOC_ABSR |= PIO_ABSR_P24 | PIO_ABSR_P21;        // Set the port C PWM pins to peripheral type B (PWM7 physical port is PWM6) | (PWM4 physical port is PWM9)
  REG_PIOC_PDR |= PIO_PDR_P24  | PIO_PDR_P21;          // Set the port C PWM pins to outputs           (PWM7 physical port is PWM6) | (PWM4 physical port is PWM9)
  REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);     // Set the PWM clock A rate to 84MHz (84MHz/1)

  PWM->PWM_CH_NUM[7].PWM_CMR =  PWM_CMR_CPRE_CLKA;     // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[7].PWM_CPRD = PERIODO;                // Set the PWM period register 84MHz/(PERIOD)= Freq
  PWM->PWM_CH_NUM[7].PWM_CDTY = PERIODO / 2;            // Duty recommended starting at 0 or (Vref/Vin) * PERIOD as the Vin is variable not using here

  PWM->PWM_CH_NUM[4].PWM_CMR =  PWM_CMR_CPRE_CLKA;     // Enable single slope PWM and set the clock source as CLKA
  PWM->PWM_CH_NUM[4].PWM_CPRD = PERIODO;                // Set the PWM period register 84MHz/(PERIOD)= Freq
  PWM->PWM_CH_NUM[4].PWM_CDTY = PERIODO / 2;            // Duty recommended starting at 0 or (Vref/Vin) * PERIOD as the Vin is variable not using here

  REG_PWM_ENA = PWM_ENA_CHID7 | PWM_ENA_CHID4; // Enable PWM CH 7 e 4

  // Configuração ADC, dois canais no modo de leitura livre
  PMC->PMC_PCER1 |= PMC_PCER1_PID37;   // ADC power on
  ADC->ADC_CR = ADC_CR_SWRST;          // Reset ADC
  ADC->ADC_MR |=  ADC_MR_FREERUN;      // Enable free run
  ADC->ADC_ACR = ADC_ACR_IBCTL(0b01);  // Only if 1 MHz > conversion frequency > 500 KHz

  ADC->ADC_CHDR = 0xFFFF; //Disable all adc channels
  ADC->ADC_CHER |= ADC_CHER_CH0 | ADC_CHER_CH7; // Enable Channels 0 and 7 physical port A0 and A7

  // Habilita a porta do Led
  PIOC->PIO_PER |= PIO_PC6; //pin 38 Led on
  PIOC->PIO_OER |= PIO_PC6;
  // Pisca o led pra indicar que esta pronto
  for (int i = 0; i < 3; i++) {
      ledon();
      delay(500);
      ledoff();
      delay(500);
  }
}

//Led liga
void ledon() {
  PIOC->PIO_SODR |= PIO_PC6;
}

//Led Desliga
void ledoff() {
  PIOC->PIO_CODR |= PIO_PC6;
}

//Loop pra sempre e verifica
void loop() {

  if (check_inicio) {
    //while ((ADC->ADC_ISR & ADC_CHER_CH0) == 0); // Espera a conversão
    if (ADC->ADC_CDR[0] < VReferencia) {
      tempo_inicial = micros();
      ledon();
      check_inicio = false;
    }
  } else {
    //while ((ADC->ADC_ISR & ADC_CHER_CH7) == 0); // Espera a conversão
    if (ADC->ADC_CDR[7] < VReferencia) {
      tempo_final =  micros();
      ledoff();
      check_inicio = true;

      tempo = (float)(tempo_final - tempo_inicial) / hora_em_micro;
      // Printa o resultado 1 segundo ou 1.000.000 de microsegundos deve dar 1.08 km/h
      Serial.print(" Velocidade = ");
      Serial.print((float)(distancia / tempo), 2);
      Serial.println(" km/h");
    }
  }

//  Serial.print("v ");
//  Serial.println(ADC->ADC_CDR[7]);
//
//  delay(1000);
}
