// Radar por laço indutivo usando arduino ou or Inductive-loop detectors using arduino
#define PERIODO 1680 // 84 Mhz / 1680 = 50 Khz
#define DUTY PERIODO / 2 // 50% duty
#define VReferencia 3000 // Tensão referencia, se baixar desta muda o esstado do led
#define VReferencia2 3200

#define hora_em_micro 3600000000 // Uma hora em microsegundos, pois a leitura de tepo é em micro mas é printado em hora
#define distancia 0.0003 // 30 cm convertido em Km

// Calculo de contagem do tempo
unsigned long tempo_inicial;
unsigned long tempo_final;
float tempo;

// Calculo das velocidades
float VM; // Velocidae medida
int VC; // Velocidade considerada, é ajustada e arredondada

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
  PWM->PWM_CH_NUM[4].PWM_CDTY = DUTY;            // Duty recommended starting at 0 or (Vref/Vin) * PERIOD as the Vin is variable not using here

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
    while ((ADC->ADC_ISR & ADC_CHER_CH0) == 0); // Espera a conversão
    if (ADC->ADC_CDR[0] < VReferencia) {
      tempo_inicial = micros();
      ledon();
      check_inicio = false;
    }
  } else {
    while ((ADC->ADC_ISR & ADC_CHER_CH7) == 0); // Espera a conversão
    if (ADC->ADC_CDR[7] < VReferencia2) {
      tempo_final =  micros();
      ledoff();
      check_inicio = true;

      tempo = (float)(tempo_final - tempo_inicial) / hora_em_micro;
      VM = (float)(distancia / tempo);
      // Art. 218 CTB Tabela II
      // até 100 km/h, a tolerância é de 7km/h.
      // Se a velocidade do veículo estiver acima de 100 km/h, o “desconto” do radar é de 7%.
      // Observações:
      // 1.VM - VELOCIDADE MEDIDA (Km/h) VC - VELOCIDADE CONSIDERADA (Km/h)
      // 2. Para velocidades medidas superiores aos indicados na tabela, considerar o erro máximo
      // admissível de 7%, com arredondamento matemático para se calcular a velocidade considerada.

      // Conforme tabela II função round(valor) arredonda pra mais caso a casa decimal for >= 0.5 o contrario para menos
      if (VM > 100) VC = round(VM * 0.93); // acima de 100 km/h, tolerancia de 7 %
      else if (VM > 7) VC = round(VM - 7); // abaixo de 100 km/h e acima de 7 km/h, tolerancia de 7 km/h
      else VC = 0; // abaixo de 7 km/h zero
      
      // Printa o resultado 1 segundo ou 1.000.000 de microsegundos deve resultar em um VM = 1.08 km/h
      Serial.print("VM = ");
      Serial.print(VM, 2);
      Serial.print("\tkm/h\t| VC = ");
      Serial.print(VC);
      Serial.println("\tkm/h");
    }
  }

//  Serial.print("v ");
//  Serial.println(ADC->ADC_CDR[7]);
//
//  delay(1000);
}
