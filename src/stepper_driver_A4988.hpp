#ifndef _STEPPER_DRIVER_A4988_HPP
#define _STEPPER_DRIVER_A4988_HPP

#include <cstdint>

#include "hardware_helper/hardware_helper.hpp"
#include "tiva_utils/pin_utils.hpp"
#include "tiva_utils/pwm_utils.hpp"

namespace a4988 {
#define A4988_TOTAL_MS_PIN 3
enum class MicroStepRes { FULL, HALF, QUARTER, EIGHTH, SIXTEENTH };

typedef struct A4988Config {
  MicroStepRes microStepRes;
  bool isCCW;

  A4988Config::A4988Config(MicroStepRes microStepRes, const bool &isCCW);
} A4988Config;

// 8 pins
class StepperDriverA4988 {
private:
  uint32_t _sleepPin[tivaextra::GPIO_DESCR_LEN];
  uint32_t _stepPin[tivaextra::PWM_DESCR_LEN];
  uint32_t _msPinList[A4988_TOTAL_MS_PIN][tivaextra::GPIO_DESCR_LEN];
  uint32_t _dirPin[tivaextra::GPIO_DESCR_LEN];
  uint32_t _enablePin[tivaextra::GPIO_DESCR_LEN];
  uint32_t _resetPin[tivaextra::GPIO_DESCR_LEN];
  A4988Config _a4988Config;

public:
  StepperDriverA4988(const char sleepPin[tivaextra::PIN_CHAR_DESC_LEN],
                     const char stepPin[tivaextra::PIN_CHAR_DESC_LEN],
                     char *const msPinList[A4988_TOTAL_MS_PIN],
                     const char dirPin[tivaextra::PIN_CHAR_DESC_LEN],
                     const char enablePin[tivaextra::PIN_CHAR_DESC_LEN],
                     const char resetPin[tivaextra::PIN_CHAR_DESC_LEN],
                     const A4988Config &a4988Config);

  void reset(void);
  void changeMicroStepRes(const MicroStepRes &microStepRes);
  void dirSwitch(const bool &isCCW);
  void sleep(void);
  void enable(void);
  void disable(void);
};

} // namespace a4988
#endif