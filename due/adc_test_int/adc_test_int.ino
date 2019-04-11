int input = A1;
int led = 13;
int val;

void setup()
{
  Serial.begin(115200);
  pinMode(input, INPUT);
  pinMode(led, OUTPUT);
  // Setup all registers
  pmc_enable_periph_clk(ID_ADC); // To use peripheral, we must enable clock distributon to it
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX, ADC_STARTUP_FAST); // initialize, set maximum posibble speed
  adc_disable_interrupt(ADC, 0xFFFFFFFF);
  adc_set_resolution(ADC, ADC_12_BITS);
  adc_configure_power_save(ADC, 0, 0); // Disable sleep
  adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1); // Set timings - standard values
  adc_set_bias_current(ADC, 1); // Bias current - maximum performance over current consumption
  adc_stop_sequencer(ADC); // not using it
  adc_disable_tag(ADC); // it has to do with sequencer, not using it
  adc_disable_ts(ADC); // deisable temperature sensor
  adc_disable_channel_differential_input(ADC, ADC_CHANNEL_7);
  adc_disable_channel_differential_input(ADC, ADC_CHANNEL_6);
  adc_disable_channel_differential_input(ADC, ADC_CHANNEL_5);
  adc_disable_channel_differential_input(ADC, ADC_CHANNEL_4);
  adc_configure_trigger(ADC, ADC_TRIG_SW, 1); // triggering from software, freerunning mode
  adc_disable_all_channel(ADC);
  adc_enable_channel(ADC, ADC_CHANNEL_7);
  adc_enable_channel(ADC, ADC_CHANNEL_6);
  adc_enable_channel(ADC, ADC_CHANNEL_5);
  adc_enable_channel(ADC, ADC_CHANNEL_4); // just one channel enabled
  adc_start(ADC);
}

void loop()
{
  while (1)
  {
    PIO_Set(PIOB, PIO_PB27B_TIOB0);
    while ((adc_get_status(ADC) & ADC_ISR_DRDY) != ADC_ISR_DRDY)
    {}; //Wait for end of conversion
    PIO_Clear(PIOB, PIO_PB27B_TIOB0);
    val = ((ADC->ADC_CDR[7] + ADC->ADC_CDR[6] + ADC->ADC_CDR[5] + ADC->ADC_CDR[4])>> 1)*(3.3/8192)*10000*2; // Read ADC
    Serial.println(val);
  }
}
