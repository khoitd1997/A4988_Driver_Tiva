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

namespace a4988 {

static const float STEP_FREQ_KHZ = 500;
static const float STEP_PULSE_WIDTH_FRACTION = 0.5;

A4988Config::A4988Config(MicroStepRes microStepRes, const bool &isCCW) {
  this->isCCW = isCCW;
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
    tivaextra::gpioModeSwitch(_msPinList[msPinIndex], false);
    tivaextra::gpioPadConfig(_msPinList[msPinIndex], GPIO_STRENGTH_4MA,
                             GPIO_PIN_TYPE_STD);
  }

  tivaextra::gpioModeSwitch(_sleepPin, false);
  tivaextra::gpioModeSwitch(_dirPin, false);
  tivaextra::gpioModeSwitch(_enablePin, false);
  tivaextra::gpioModeSwitch(_resetPin, false);

  tivaextra::gpioPadConfig(_sleepPin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
  tivaextra::gpioPadConfig(_dirPin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
  tivaextra::gpioPadConfig(_enablePin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);
  tivaextra::gpioPadConfig(_resetPin, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

  // configure PWM pin
  tivaextra::initPeripheralPin(stepPin, _stepPin, GPIO_PB6_M0PWM0,
                               tivaextra::PeripheralType::PWM);
  tivaextra::pwmConfigure(
      _stepPin, SYSCTL_PWMDIV_1, STEP_FREQ_KHZ, STEP_PULSE_WIDTH_FRACTION,
      PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_DB_NO_SYNC | PWM_GEN_MODE_NO_SYNC |
          PWM_GEN_MODE_FAULT_LATCHED);
}

void StepperDriverA4988::changeMicroStepRes(const MicroStepRes &microStepRes) {
  _a4988Config.microStepRes = microStepRes;
  switch (microStepRes) {
  case MicroStepRes::FULL:
    tivaextra::gpioWrite(_msPinList[0], false);
    tivaextra::gpioWrite(_msPinList[1], false);
    tivaextra::gpioWrite(_msPinList[2], false);
    break;
  case MicroStepRes::HALF:
    tivaextra::gpioWrite(_msPinList[0], true);
    tivaextra::gpioWrite(_msPinList[1], false);
    tivaextra::gpioWrite(_msPinList[2], false);
    break;
  case MicroStepRes::QUARTER:
    tivaextra::gpioWrite(_msPinList[0], false);
    tivaextra::gpioWrite(_msPinList[1], true);
    tivaextra::gpioWrite(_msPinList[2], false);
    break;
  case MicroStepRes::EIGHTH:
    tivaextra::gpioWrite(_msPinList[0], true);
    tivaextra::gpioWrite(_msPinList[1], true);
    tivaextra::gpioWrite(_msPinList[2], false);
    break;
  case MicroStepRes::SIXTEENTH:
    tivaextra::gpioWrite(_msPinList[0], false);
    tivaextra::gpioWrite(_msPinList[1], false);
    tivaextra::gpioWrite(_msPinList[2], false);
    break;
  }
}

void StepperDriverA4988::dirSwitch(const bool &isCCW) {
  // TODO: calibrate this to make sure it's correct
  _a4988Config.isCCW = isCCW;
  tivaextra::gpioWrite(_dirPin, isCCW);
}

void StepperDriverA4988::reset(void) { tivaextra::gpioWrite(_resetPin, false); }

void StepperDriverA4988::sleep(void) { tivaextra::gpioWrite(_sleepPin, false); }

void StepperDriverA4988::disable(void) {
  tivaextra::gpioWrite(_enablePin, true);
}

void StepperDriverA4988::enable(void) {
  tivaextra::gpioWrite(_sleepPin, true);
  tivaextra::gpioWrite(_resetPin, true);

  changeMicroStepRes(_a4988Config.microStepRes);
  dirSwitch(_a4988Config.isCCW);

  tivaextra::pwmEnable(_stepPin, false);

  tivaextra::gpioWrite(_enablePin, false);
}
} // namespace a4988