/*
  Interface between Arduino DM board and Linear Tech LTC2440 24-bit ADC
  Nov. 12 2010 John Beale

   LTC2440  <---------->  Arduino
   11: /CS <- to digital pin 10  (SS pin)
    7: MOSI <- to digital pin 11 (MOSI pin)
   12: MISO -> to digital pin 12 (MISO pin)
   13: SCLK <- to digital pin 13 (SCK pin)
   10: /EXT - ground
    1: GND - ground
    2: VDD3 - 5V supply

*/

#include <SPI.h>  // include the SPI library

// set I/O pins used in addition to clock, data in, data out
const byte slaveSelectPin = 10;  // digital pin 10 for /CS
const byte resetPin = 9;  // digital pin 9 for /RESET

const int nsamples = 5;  // how many ADC readings to average together

// SPI_CLOCK_DIV16 gives me a 1.0 MHz SPI clock, with 16 MHz crystal on Arduino

void setup() {

  Serial.begin(9600);      // set up serial comm to PC at this baud rate

  pinMode (slaveSelectPin, OUTPUT);
  pinMode (resetPin, OUTPUT);
  digitalWrite(slaveSelectPin, HIGH); // chip select is active low
  digitalWrite(resetPin, HIGH); // reset is active low

  SPI.begin(); // initialize SPI, covering MOSI,MISO,SCK signals
  SPI.setBitOrder(MSBFIRST);  // data is clocked in MSB first
  SPI.setDataMode(SPI_MODE0);  // SCLK idle low (CPOL=0), MOSI read on rising edge (CPHI=0)
  SPI.setClockDivider(SPI_CLOCK_DIV16);  // system clock = 16 MHz, chip max = 1 MHz

  Serial.println("LTC2440 Test");
}

// =============================================================================
// Main Loop:
// acquire 'nsamples' readings, convert to units of volts, and send out on serial port

void loop() {

  int i;
  int time;
  double volts;
  long in;         // incoming serial 32-bit word
  long sum = 0;

  for (i = 0; i < nsamples; i++) {
    in = SpiRead();
    in &= 0x1FFFFFFF; // force high three bits to zero
    in = in >> 5; // truncate lowest 5 bits
    sum += in;
    delay(198);      // (msec). Total Looptime: +2 msec (overhead for comms)
  }

  // volts =  in * 2.5 / 8.388607;  // 0x7fffff = 8388607
  volts = sum * (0.2980232594);  // microvolts
  volts = volts / nsamples;

  time = micros();   // elapsed time in minutes
  Serial.print(time);
  Serial.print(", ");
  Serial.println(volts);

} // end main loop


// =================================================================
// SpiRead() -- read out 4 bytes from LTC2440 chip via SPI interface
// =================================================================

long SpiRead(void) {

  long result = 0;
  long b;

  //  long result2 = 0;// MOSI/SDI pin 7 HIGH => 7 Hz, best resolution

  digitalWrite(slaveSelectPin, LOW);  // take the SS pin low to select the chip
  delayMicroseconds(1);              // probably not needed, only need 25 nsec delay

  b = SPI.transfer(0xff);   // B3
  result = b << 8;
  b = SPI.transfer(0xff);   // B2
  result |= b;
  result = result << 8;
  b = SPI.transfer(0xff);   // B1
  result |= b;
  result = result << 8;
  b = SPI.transfer(0xff);   // B0
  result |= b;

  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);
  return (result);
}
