//Habilita escrita na porta antes do primeiro upload
//sudo chmod a+rw /dev/ttyUSB0

#define PWM_ANTENA PB3 //Porta 11 arduino

//
void setup() {

  //Geração de frequência da portadora
  uint32_t FREQ_ANTENA = 800;                       //KHz
  DDRB |= (1 << PWM_ANTENA);
  TCCR2A = (0 << COM2A1) | (1 << COM2A0);           //Toggle OC0A
  TCCR2A |= (1 << WGM21) | (0 << WGM20);            //CTC
  TCCR2B = (0 << CS22) + (0 << CS21) + (1 << CS20); //Sem Prescaling
  OCR2A = F_CPU / (2000 * FREQ_ANTENA) - 1;

  char strbuf[255];
  sprintf(
    strbuf,
    "FREQ_ANTENA %d KHz",
    (F_CPU / (2 * (1 + OCR2A)) / 1000)
  );
  Serial.begin(9600);
  Serial.println(strbuf);

  //Configura PWM
  //TCCR1A |= (0 << WGM11) | (1 << WGM10);            //Fast PWM 8 Bit
  //TCCR1A |= (1 << WGM11) | (0 << WGM10);            //Fast PWM 9 Bit
  TCCR1A |= (1 << WGM11) | (1 << WGM10);              //Fast PWM 10 Bit
  TCCR1B = (1 << WGM12);
  TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);  //Limpa prescaler bits
  TIMSK1 = (1 << OCIE1A) | (1 << TOIE1);
  sei();//allow interrupts

  //Configura ADC
  ADMUX = (1 << REFS1) | (1 << REFS0);                  //Referencia interna 1.1V

  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2)); // Limpa prescaler bits

  // sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
  // for Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles
  ADCSRA |= bit (ADPS1) | bit (ADPS2);        // 64 prescaler for 19.2 KHz
  //ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
  //ADCSRA |= (1 << ADPS2);                   // 16 prescaler for 76.9 KHz
  //ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz
  //ADCSRA |= bit (ADPS1);                    // 4 prescaler for 300 KHz

  ADCSRA |= (1 << ADATE); // enable auto trigger
  ADCSRA |= (1 << ADEN);  // enable ADC
  ADCSRA |= (1 << ADSC);  // start ADC measurements

  DIDR0 |= (1 << ADC0D);  //Habilita ADC porta 0
}

//Interrupção de leitura do ADC
//A cada interrupção o valor do ADC é lido o PWM é habilitado e seta quando a função ISR(TIMER1_COMPA_vect) sera chamada
ISR(TIMER1_OVF_vect) {
  //Copia da função analogRead() sem o que não precisa...
  // we have to read ADCL first; doing so locks both ADCL
  // and ADCH until ADCH is read.  reading ADCL second would
  // cause the results of each conversion to be discarded,
  // as ADCL and ADCH would be locked when it completed.
  uint8_t low, high;
  low  = ADCL;
  high = ADCH;

  //Copia o valor da leitura do ADC para comparador
  //Quanto maior for a amplitude do sinal lido, mais tempo levara para que a função
  //ISR(TIMER1_COMPA_vect) seja chamada
  //Assim gerando a modulação
  OCR1A = (high << 8) | low;
  //Habilita o pwm
  DDRB |= (1 << PWM_ANTENA);
}

//Interrupção de comparação
ISR(TIMER1_COMPA_vect) {
  //desabilita o pwm
  DDRB &= ~(1 << PWM_ANTENA);
}

void loop() {
}

