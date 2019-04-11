
void setup()
{


  adc_setup();

}


void loop()
{

  int16_t Diff67Value;
  Diff67Value =  ADC->ADC_LCDR;

}


void adc_setup() {

  PMC->PMC_PCER1 |= PMC_PCER1_PID37;      // ADC power on
  ADC->ADC_CR = ADC_CR_SWRST;            // Reset ADC

  ADC->ADC_MR |=  ADC_MR_FREERUN
                  | ADC_MR_ANACH_ALLOWED;

  ADC->ADC_CGR =  ADC_CGR_GAIN7 | ADC_CGR_GAIN6//;0xF000;

  ADC->ADC_COR =  ADC_COR_DIFF7 // Differential mode for channel 7 and channel 6 (6 - 7); Gain = 0.5
                  | ADC_COR_DIFF6
                  | ADC_COR_OFF6  // Offset is mandatory
                  | ADC_COR_OFF7;

  ADC->ADC_CHER |=  ADC_CHER_CH6 | ADC_CHER_CH7; // Enable Channels 7,6 = A0,A1  

}





