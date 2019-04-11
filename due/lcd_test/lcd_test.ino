#include <LiquidCrystalDue.h>

LiquidCrystalDue lcd(43, 42, 34, 35, 36, 37, 38, 39, 40, 41);

uint32_t NUMBER[12] = {0xC0, 0xC4, 0xC8, 0xCC, 0xD0, 0xD4, 0xD8, 0xDC, 0xE0, 0xE4, 0xF0, 0x80}; // the last two are < and empty
uint32_t CURSOR_0[16] = {0x200, 0x204, 0x208, 0x20C, 0x210, 0x214, 0x218, 0x21C, 0x220, 0x224, 0x228, 0x22C, 0x230, 0x234, 0x238, 0x23C};
uint32_t CURSOR_1[16] = {0x300, 0x304, 0x308, 0x30C, 0x310, 0x314, 0x318, 0x31C, 0x320, 0x324, 0x328, 0x32C, 0x330, 0x334, 0x338, 0x33C};

unsigned long start_time;
unsigned long stop_time;
int i = 0;

float num = 30.15;
char  buf[4];

void setup()
{
  Serial.begin ( 9600 );

  lcd.begin(16, 2);              // initialize the lcd

  lcd.home ();                   // go home
  lcd.print("V: 12.00 ");
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print("A: 10.00");
  lcd.noAutoscroll();
}

int lcd_delay = 37;

void movecursor(uint32_t pos)
{
  PIOA->PIO_CODR = PIO_PA20;//Rs
  PIOC->PIO_CODR = 0xFFFF;//Rest port D0 a D7
  PIOC->PIO_SODR = pos;//set number to port D0 a D7

  PIOA->PIO_SODR = PIO_PA19;//ena up
  //delayMicroseconds(1);
  PIOA->PIO_CODR = PIO_PA19;//ena down
  delayMicroseconds(lcd_delay);
}

void printlcd(uint32_t val)
{
  PIOA->PIO_SODR = PIO_PA20;//Rs
  PIOC->PIO_CODR = 0xFFFF;//Rest port D0 a D7
  PIOC->PIO_SODR = val;//set number to port D0 a D7

  PIOA->PIO_SODR = PIO_PA19;//ena up
  //delayMicroseconds(1 / 2);
  PIOA->PIO_CODR = PIO_PA19;//ena down
  //after 37 micro aparece
  delayMicroseconds(lcd_delay);
}

void loop()
{
  //  Serial.println("7 1");
  //  lcd.setCursor ( 7, 1 );
  //  Serial.println("7 1 end");
  delay(1000);

  movecursor(CURSOR_0[6]);
  printlcd(NUMBER[5]);

  movecursor(CURSOR_0[7]);
  printlcd(NUMBER[9]);

  movecursor(CURSOR_1[7]);
  printlcd(NUMBER[1]);

  movecursor(CURSOR_1[9]);
  printlcd(NUMBER[10]);
  movecursor(CURSOR_1[9]);
  printlcd(NUMBER[11]);

  char a[2] = {0,0}, b[2] = {0,0}, c[2] = {0,0}, d[2] = {0,0};
  int  x, y, z, w;

  sprintf(buf, "%05.2f", num);

  a[0] = buf[0];
  b[0] = buf[1];
  c[0] = buf[3];
  d[0] = buf[4];

  //convert back into numeric discretes if necessary
  x = atoi(a);
  y = atoi(b);
  z = atoi(c);
  w = atoi(d);

  Serial.print("x = ");
  Serial.println(x);
  Serial.print("y = ");
  Serial.println(y);
  Serial.print("z = ");
  Serial.println(z);
  Serial.print("w = ");
  Serial.println(w);

  exit(0);
}
