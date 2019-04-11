
int PERIOD = 420; // 84MHz/420 = 200Khz
int DUTY = 0; // start at 50%
int PWM_PIMS[2] = {6, 7};// PIM_BUCK, PIM_BOOST
int lcd_delay = 37;
int conter = 0;
int i = 0;

float volts = 0;
char voltsToString[2] = {0, 0};
char buf[4];
unsigned long start_time;
unsigned long stop_time;
#include <LiquidCrystalDue.h>

LiquidCrystalDue lcd(43, 42, 34, 35, 36, 37, 38, 39, 40, 41);

uint32_t NUMBER[12] = {0xC0, 0xC4, 0xC8, 0xCC, 0xD0, 0xD4, 0xD8, 0xDC, 0xE0, 0xE4, 0xF0, 0x80}; // the last two are < and empty
uint32_t CURSOR_0[16] = {0x200, 0x204, 0x208, 0x20C, 0x210, 0x214, 0x218, 0x21C, 0x220, 0x224, 0x228, 0x22C, 0x230, 0x234, 0x238, 0x23C};
uint32_t CURSOR_1[16] = {0x300, 0x304, 0x308, 0x30C, 0x310, 0x314, 0x318, 0x31C, 0x320, 0x324, 0x328, 0x32C, 0x330, 0x334, 0x338, 0x33C};

void setup () {
  Serial.begin(9600);

  ADC->ADC_MR |= 0x80;  //set free running mode on ADC and 12-bit resolution | pdf page 1345
  ADC->ADC_CHDR = 0xFFFFFFFF; //Disable all channel to be shore | pdf page 1351
  ADC->ADC_CHER = 0xE0; //enable ADC on pin A0 | pdf page 1350

  lcd.begin(16, 2);              // initialize the lcd
  lcd.home ();                   // go home
  lcd.print("V: 12.00 ");
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print("A: 10.00");
  lcd.noAutoscroll();
}


void movecursor(uint32_t pos)
{
  PIOA->PIO_CODR = PIO_PA20;//Rs
  PIOC->PIO_CODR = 0xFFFF;//Rest port D0 a D7
  PIOC->PIO_SODR |= pos;//set number to port D0 a D7

  PIOA->PIO_SODR = PIO_PA19;//ena up
  //delayMicroseconds(1);
  PIOA->PIO_CODR = PIO_PA19;//ena down
  delayMicroseconds(lcd_delay);
}

void printlcd(uint32_t val)
{
  PIOA->PIO_SODR = PIO_PA20;//Rs
  PIOC->PIO_CODR = 0xFFFF;//Rest port D0 a D7
  PIOC->PIO_SODR |= val;//set number to port D0 a D7

  PIOA->PIO_SODR = PIO_PA19;//ena up
  //delayMicroseconds(1 / 2);
  PIOA->PIO_CODR = PIO_PA19;//ena down
  //after 37 micro aparece
  delayMicroseconds(lcd_delay);
}

void upvolts(float volts)
{
  char a[2] = {0, 0}, b[2] = {0, 0}, c[2] = {0, 0}, d[2] = {0, 0};
  int  x, y, z, w;

  sprintf(buf, "%05.2f", volts);

  a[0] = buf[0];
  b[0] = buf[1];
  c[0] = buf[3];
  d[0] = buf[4];

  //convert back into numeric discretes if necessary
  x = atoi(a);
  y = atoi(b);
  z = atoi(c);
  w = atoi(d);

  movecursor(CURSOR_0[3]);
  if (x) printlcd(NUMBER[x]);
  else printlcd(NUMBER[11]);

  movecursor(CURSOR_0[4]);
  printlcd(NUMBER[y]);

  movecursor(CURSOR_0[6]);
  printlcd(NUMBER[z]);

  movecursor(CURSOR_0[7]);
  printlcd(NUMBER[w]);
}

void loop() {

  while ((ADC->ADC_ISR & 0xE0) == 0); // wait for conversion
  volts = ADC->ADC_CDR[7]; //get values

  Serial.print("volts = ");
  Serial.println((int)volts);
  //upvolts(volts * ((3.3 / 4096.0) * 5.0));

  delay(10);
  //exit(0);
}
