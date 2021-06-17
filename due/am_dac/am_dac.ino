/****************************************************************************************************/
/*                                                                                                  */
/*       Transmissor AM usando Arduino DUE, um ADC, DAC e Timer                                     */
/*       É gerada uma portadora de onda quadrada usando o timer TIOA2 operando em 1.4MHz            */
/*       Em pulsos impares do timer é zerada o DAC                                                  */
/*       Em pulsos pares é lido o ADC e setado este valor no DAC                                    */
/*       Assim as conversões do ADC são a uma frequência de 700 kHz                                 */
/*       E o DAC é modificado a uma frequência de 1.4 MHz                                            */
/*                                                                                                  */
/****************************************************************************************************/

// Frequência de operação, não é um valor exato mas aproximado para determinar o divisor de frequência, veja tc_setup() para entender como é usado
// Frequência máxima 807.44 kHz, valores maiores o DAC não vai conseguir reagir
#define FREQ 700

void setup()
{

  adc_setup();
  dac_setup();
  tc_setup();
}

void loop()
{
  //Nada é feito aqui, tudo acontece na Função de interrupção do ADC
}

/*************  Configura o ADC  *******************/
void adc_setup()
{

  PMC->PMC_PCER1 |= PMC_PCER1_PID37;       // Habilita o ADC

  ADC->ADC_CR = ADC_CR_SWRST;              // Reseta ADC
  ADC->ADC_MR |= ADC_MR_TRGEN_EN           // Habilita o tipo de trigger acionamento do ADC para HW
                 | ADC_MR_TRGSEL_ADC_TRIG3 // Seta o acionamento pelo timer TIOA2
                 | ADC_MR_PRESCAL(1);      // Seta o divisor de frequência para 1, sem divisão frequência livre em relação ao timer

  ADC->ADC_ACR = ADC_ACR_IBCTL(0b01);      // Para o operações em frequências maiores que > 500 KHz de leitura, permite o ADC de usar a corrente máxima disponível

  ADC->ADC_IER = ADC_IER_EOC7;             // Habilita interrupção no ADC 7 porta física A0
  NVIC_EnableIRQ(ADC_IRQn);                // Habilita interrupção de ADC
  ADC->ADC_CHER = ADC_CHER_CH7;            // Habilita o ADC 7 porta física A0
}

/*************  Função de interrupção do ADC  *******************/
void ADC_Handler()
{

  static bool le_adc = true;

  DACC->DACC_CDR = le_adc ? ADC->ADC_CDR[7] : 0; // Se le_adc = true Lê ADC->ADC_CDR[7] para o DAC, caso não DAC = 0

  le_adc = !le_adc; // Inverte le_adc
}

/*************  Configura o DAC  *******************/
void dac_setup()
{

  PMC->PMC_PCER1 = PMC_PCER1_PID38;           // Habilita DACC

  DACC->DACC_CR = DACC_CR_SWRST;              // Reset DACC
  DACC->DACC_MR = DACC_MR_TRGEN_EN            // Habilita o tipo de trigger acionamento do ADC para HW
                  | DACC_MR_TRGSEL(0b011)     // Seta o acionamento pelo timer TIOA2
                  | DACC_MR_USER_SEL_CHANNEL1 // seleciona o canal 1 do dac
                  | DACC_MR_REFRESH(1)        // Seta o divisor de frequência para 1, sem divisão frequência livre em relação ao timer
                  | DACC_MR_STARTUP_8         // Define o tempo de inicialização do DAC 8 = 512 periodos do DACClock
                  | DACC_MR_MAXS;             // Habilita o modo de máxima velocidade de DAC

  DACC->DACC_CHER = DACC_CHER_CH1;            // Habilita o canal 1, porta física DAC1
}

/*************  Contador Timer canal 2, gera pulsos de PWM pelo timer TIOA2  ************/
void tc_setup()
{

  PMC->PMC_PCER0 |= PMC_PCER0_PID29; // Habilita TC2: O contador Timer 0 e canal 2 é o TC2

  TC0->TC_CHANNEL[2].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1 // Seta o clock base, em relação ao master clock, neste caso MCK/2 (84 MHz / 2), cada pulso na borda de subida
                              | TC_CMR_WAVE              // Habilita o seleção de modo de onda - Waveform mode - possibilita criação de ate dois PWM com controle de Duty
                              | TC_CMR_WAVSEL_UP_RC      // Seleciona o modo da onda, UP mode with automatic trigger on RC Compare
                              | TC_CMR_ACPA_CLEAR        // Limpa automaticamente o registro RA a cada confirmação de comparação
                              | TC_CMR_ACPC_SET;         // Seta o timer TIOA2 para confirmação de comparação no registro RC

  float freq_rc = 42000 / (FREQ * 2);                    // Calcula o valor do divisor da frequência a ser usada, 42000 (é p clock base 84 MHz / 2 em kHz), 42000 / (700 * 2) = 30
  TC0->TC_CHANNEL[2].TC_RC = freq_rc;                    // Seta a frequência do timer = (Mck/2)/TC_RC  Hz (84 MHz/2) / 30 = 1.4 MHz
  TC0->TC_CHANNEL[2].TC_RA = freq_rc / 2;                // Seta o duty do timer, um valor maior que 1 menor que freq_rc, neste caso 50%

  TC0->TC_CHANNEL[2].TC_CCR = TC_CCR_SWTRG              // Ativa o timer por Software, os contadores são resetados e o timer é iniciado
                              | TC_CCR_CLKEN;           // Habilita o timer
}
