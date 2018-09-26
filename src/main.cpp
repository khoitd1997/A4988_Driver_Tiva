#include "stepper_driver_A4988.hpp"

// peripheral
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

// hardware
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "general_timer/general_timer.hpp"
#include "hardware_helper/hardware_helper.hpp"
#include "tiva_utils/pin_utils.hpp"
#include "tiva_utils/pwm_utils.hpp"

int main(void) {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                 SYSCTL_OSC_MAIN);

  // auto a4988Config = a4988::A4988Config(a4988::MicroStepRes::HALF, true);
  // char *msList[] = {"F3", "F2", "D3"};
  // auto stepperDriver = a4988::StepperDriverA4988("C4", "B6", msList, "C6",
  // "C7",
  //                                                "F1", a4988Config);
  // stepperDriver.enable();

  auto generalTimer = GeneralTimer(UNIT_NANOSEC);

  float pwmFreqKhz = 3;
  float pwmPulseWidthFraction = 0.5;
  float riseTimeDelayFraction = 0.125;
  float fallTimeDelayFraction = 0.125;
  uint32_t pwmPin1[tivaextra::PWM_DESCR_LEN] = {0};
  uint32_t pwmPin2[tivaextra::PWM_DESCR_LEN] = {0};

  uint32_t pwmPair1Setting = PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC;
  uint32_t pwmClockDiv = SYSCTL_PWMDIV_1;
  tivaextra::initPeripheralPin("B6", pwmPin1, GPIO_PB6_M0PWM0,
                               tivaextra::PeripheralType::PWM);
  tivaextra::pwmConfigure(pwmPin1, pwmClockDiv, pwmFreqKhz,
                          pwmPulseWidthFraction, pwmPair1Setting);

  tivaextra::initPeripheralPin("B7", pwmPin2, GPIO_PB7_M0PWM1,
                               tivaextra::PeripheralType::PWM);
  tivaextra::pwmConfigure(pwmPin2, pwmClockDiv, pwmFreqKhz,
                          pwmPulseWidthFraction, pwmPair1Setting);

  tivaextra::pwmConfigureDeadband(pwmPin1, true, riseTimeDelayFraction,
                                  fallTimeDelayFraction, pwmFreqKhz);
  tivaextra::pwmEnable(pwmPin1, true);

  uint32_t pwmPin3[tivaextra::PWM_DESCR_LEN] = {0};
  uint32_t pwmPin4[tivaextra::PWM_DESCR_LEN] = {0};

  uint32_t pwmPair2Setting = PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC;
  tivaextra::initPeripheralPin("E4", pwmPin3, GPIO_PE4_M0PWM4,
                               tivaextra::PeripheralType::PWM);
  tivaextra::pwmConfigure(pwmPin3, pwmClockDiv, pwmFreqKhz,
                          pwmPulseWidthFraction, pwmPair2Setting);

  tivaextra::initPeripheralPin("E5", pwmPin4, GPIO_PE5_M0PWM5,
                               tivaextra::PeripheralType::PWM);
  tivaextra::pwmConfigure(pwmPin4, pwmClockDiv, pwmFreqKhz,
                          pwmPulseWidthFraction, pwmPair2Setting);

  tivaextra::pwmConfigureDeadband(pwmPin3, true, riseTimeDelayFraction,
                                  fallTimeDelayFraction, pwmFreqKhz);

  tivaextra::pwmEnable(pwmPin3, true);
  // tivaextra::pwmEnable(pwmPin4, false);

  PWMGenEnable(pwmPin1[tivaextra::PWM_DESCR_MODULE_INDEX],
               pwmPin1[tivaextra::PWM_DESCR_GEN_INDEX]);
  // generalTimer.wait(13000);
  PWMGenEnable(pwmPin3[tivaextra::PWM_DESCR_MODULE_INDEX],
               pwmPin3[tivaextra::PWM_DESCR_GEN_INDEX]);
  // PWMGenEnable(pwmPin4[tivaextra::PWM_DESCR_MODULE_INDEX],
  //              pwmPin4[tivaextra::PWM_DESCR_GEN_INDEX]);

  for (;;) {
    // process loop
  }
  return 0;
}
