#include <LiquidCrystalDue.h>

//Define  LCD port   RS  EN  D4  D5  D6  D7
//Arduino Port PIN   C19 C8  D9  D3  D1  D0
LiquidCrystalDue lcd(44, 40, 30, 28, 26, 25);//Arduino physical PIN

// Update the number bellow is relation to Arduino choosed ports
// Values for 0 to 15 in relation to ports D4 - D7
#define NUMBER_0 0x0
#define NUMBER_1 0x200
#define NUMBER_2 0x8
#define NUMBER_3 0x208
#define NUMBER_4 0x2
#define NUMBER_5 0x202
#define NUMBER_6 0xA
#define NUMBER_7 0x20A
#define NUMBER_8 0x1
#define NUMBER_9 0x201
#define NUMBER_10 0x9
#define NUMBER_11 0x209
#define NUMBER_12 0x3
#define NUMBER_13 0x203
#define NUMBER_14 0xB
#define NUMBER_15 0x20B

#define LCD_DELAY   37         //minimum LCD delay between commands
#define LINHA_1     NUMBER_8   //Line 1 hex value in relation to ports D4 - D7
#define LINHA_2     NUMBER_12  //Line 2 hex value in relation to ports D4 - D7

// A array to make a simple "int" call to the position
uint32_t POSITION[16] = {
  NUMBER_0,
  NUMBER_1,
  NUMBER_2,
  NUMBER_3,
  NUMBER_4,
  NUMBER_5,
  NUMBER_6,
  NUMBER_7,
  NUMBER_8,
  NUMBER_9,
  NUMBER_10,
  NUMBER_11,
  NUMBER_12,
  NUMBER_13,
  NUMBER_14,
  NUMBER_15
};

// A array to make a simple "int" call to the position
// NUMBER_3 is the base number to set a number, NUMBER_2 to set a empty char
uint32_t NUMBER[12][2] = {
  {NUMBER_3, NUMBER_0},
  {NUMBER_3, NUMBER_1},
  {NUMBER_3, NUMBER_2},
  {NUMBER_3, NUMBER_3},
  {NUMBER_3, NUMBER_4},
  {NUMBER_3, NUMBER_5},
  {NUMBER_3, NUMBER_6},
  {NUMBER_3, NUMBER_7},
  {NUMBER_3, NUMBER_8},
  {NUMBER_3, NUMBER_9},
  {NUMBER_2,    NUMBER_0}, //empty character
  {NUMBER_3, NUMBER_12}    //less-than character <
};

float volts = 0;
char voltsToString[2] = {0, 0};
char buf[4];

void setup () {
  Serial.begin(115200);
  // initialize the lcd
  lcd.begin(16, 2);
  lcd.home ();
  lcd.print("BOOST V: 12.00 <");
  lcd.setCursor ( 0, 1 );
  lcd.print("D: 60 A: 11.00  ");
  lcd.noAutoscroll();

  //LCD configuration
  PIOA->PIO_PER |= PIO_PA14;
  PIOA->PIO_OER |= PIO_PA14;
  PIOA->PIO_SODR |= PIO_PA14;//pin 23 LCD brigthness
}

void movecursor(uint32_t linha, int pos)
{
  PIOC->PIO_CODR |= PIO_PC19;//RS Down

  setPorts(linha);        //Set the line
  setPorts(POSITION[pos]);//Set the position

  delayMicroseconds(LCD_DELAY);
}

void printlcd(int val)
{
  PIOC->PIO_SODR |= PIO_PC19;//RS Up
  
  setPorts(NUMBER[val][0]);//Set the base
  setPorts(NUMBER[val][1]);//Set the val

  //after 37 micro aparece
  delayMicroseconds(LCD_DELAY);
}

void setPorts(uint32_t val)
{
    PIOD->PIO_CODR |= NUMBER_15;//Reset port D4 a D7
    PIOD->PIO_SODR |= val;//set number to port D4 a D7
    PIOC->PIO_SODR |= PIO_PC8;//ena up
    PIOC->PIO_CODR |= PIO_PC8;//ena down
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

  //  movecursor(CURSOR_0[3]);
  //  if (x) printlcd(x);
  //  else printlcd(11);
  //
  //  movecursor(CURSOR_0[4]);
  //  printlcd(y);
  //
  //  movecursor(CURSOR_0[6]);
  //  printlcd(z);
  //
  //  movecursor(CURSOR_0[7]);
  //  printlcd(w);
}

void loop() {
  delay(1000);
  for (int i = 0; i < 16; i++) {
    movecursor(LINHA_1, i);
    printlcd(9);
  }

  for (int i = 0; i < 16; i++) {
    movecursor(LINHA_2, i);
    printlcd(9);
  }
  delay(2000);

  movecursor(LINHA_1, 0);
  for (int i = 0; i < 12; i++) {
    printlcd(i);
  }
  printlcd(10);
  printlcd(10);
  printlcd(10);
  printlcd(10);

  movecursor(LINHA_2, 0);
  for (int i = 0; i < 12; i++) {
    printlcd(i);
  }
  printlcd(10);
  printlcd(10);
  printlcd(10);
  printlcd(10);
  delay(1000);
  exit(0);
}
