
#include "pwm_lib.h"
#include "tc_lib.h"

using namespace arduino_due::pwm_lib;

// defining pwm object using pin 35, pin PC3 mapped to pin 35 on the DUE
// this object uses PWM channel 0
pwm<pwm_pin::PWMH0_PC3> pwm_pin35;
uint32_t duty_value= 100; // hundredth of usecs (1e-8 secs)

#define PWM_PERIOD_PIN_35 334 // value time (1e-8 secs) 300Khz
#define PWM_DUTY_PIN_35 167 // hundredth of usecs (1e-8 secs)

void setup() {
  pwm_pin35.start(PWM_PERIOD_PIN_35,PWM_DUTY_PIN_35);
}

void loop() {
  duty_value++;
  pwm_pin35.set_duty(duty_value);
  duty_value--;
}
