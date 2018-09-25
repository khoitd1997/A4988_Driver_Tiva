#include "stepper_driver_A4988.hpp"
#include "hardware_helper/hardware_helper.hpp"
#include "tiva_utils/pin_utils.hpp"
#include "tiva_utils/pwm_utils.hpp"

// peripheral
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/udma.h"

// hardware
#include "inc/hw_memmap.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"

static const uint32_t PWM_PERIOD_TICKS = 400;
static const float PWM_PULSE_WIDTH_PERCENT = 0.50;
static const uint32_t PWM_PULSE_WIDTH_TICKS =
    PWM_PERIOD_TICKS * PWM_PULSE_WIDTH_PERCENT;

namespace a4988 {

A4988Config::A4988Config(MicroStepRes microStepRes) {
  this->microStepRes = microStepRes;
}

StepperDriverA4988::StepperDriverA4988(
    const char sleepPin[tivaextra::PIN_CHAR_DESC_LEN],
    const char stepPin[tivaextra::PIN_CHAR_DESC_LEN],
    char *const msPinList[A4988_TOTAL_MS_PIN],
    const char dirPin[tivaextra::PIN_CHAR_DESC_LEN],
    const char enablePin[tivaextra::PIN_CHAR_DESC_LEN],
    const char resetPin[tivaextra::PIN_CHAR_DESC_LEN],
    const A4988Config &a4988Config)
    : _a4988Config(a4988Config), _sleepPin{0}, _stepPin{0},
      _msPinList{{0}}, _dirPin{0}, _enablePin{0}, _resetPin{0} {

  tivaextra::initPeripheralPin(sleepPin, _sleepPin, 0,
                               tivaextra::PeripheralType::GPIO_OUTPUT);

  for (uint32_t msPinIndex = 0; msPinIndex < A4988_TOTAL_MS_PIN; ++msPinIndex) {
    tivaextra::initPeripheralPin(msPinList[msPinIndex], _msPinList[msPinIndex],
                                 0, tivaextra::PeripheralType::GPIO_OUTPUT);
  }

  tivaextra::initPeripheralPin(dirPin, _dirPin, 0,
                               tivaextra::PeripheralType::GPIO_OUTPUT);
  tivaextra::initPeripheralPin(enablePin, _enablePin, 0,
                               tivaextra::PeripheralType::GPIO_OUTPUT);
  tivaextra::initPeripheralPin(resetPin, _resetPin, 0,
                               tivaextra::PeripheralType::GPIO_OUTPUT);

  // configure GPIO type pins
  for (uint32_t msPinIndex = 0; msPinIndex < A4988_TOTAL_MS_PIN; ++msPinIndex) {
    tivaextra::pinModeSwitch(_msPinList[msPinIndex], false);
    tivaextra::pinPadConfig(_msPinList[msPinIndex], GPIO_STRENGTH_4MA,
                            GPIO_PIN_TYPE_STD);
  }

  tivaextra::pinModeSwitch(_sleepPin, false);
  tivaextra::pinModeSwitch(_dirPin, false);
  tivaextra::pinModeSwitch(_enablePin, false);
  tivaextra::pinModeSwitch(_resetPin, false);

  tivaextra::pinPadConfig(_sleepPin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
  tivaextra::pinPadConfig(_dirPin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
  tivaextra::pinPadConfig(_enablePin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
  tivaextra::pinPadConfig(_resetPin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

  // configure PWM pin
  tivaextra::initPeripheralPin(stepPin, _stepPin, GPIO_PB6_M0PWM0,
                               tivaextra::PeripheralType::PWM);
}

void StepperDriverA4988::enable(void) {
  tivaextra::pwmConfigure(_stepPin, SYSCTL_PWMDIV_16, 50, 50);
  tivaextra::pwmEnable(_stepPin);
}
} // namespace a4988