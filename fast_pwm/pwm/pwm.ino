

#include <TimerOne.h>


#define PWM_PIN 9        // the output pin for the pwm 
int timer = 50;
int duty = 50;


void setup() 
{
  Timer1.initialize(timer);                 // set a timer of length 20 microseconds
  Timer1.pwm(PWM_PIN, duty);              // setup pwm on pin 9, 50% duty cycle
}

void loop()
{
//delay(10);
//duty-=400;
//Timer1.pwm(PWM_PIN, duty);
//
//delay(10);
//duty+=400;
//Timer1.pwm(PWM_PIN, duty);
//
//delay(10);
//duty+=400;
//Timer1.pwm(PWM_PIN, duty);
//
//delay(10);
//duty-=400;
//Timer1.pwm(PWM_PIN, duty);
}
