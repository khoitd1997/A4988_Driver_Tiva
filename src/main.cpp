#include "stepper_driver_A4988.hpp"

// peripheral
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

// hardware
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

int main(void) {
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                 SYSCTL_OSC_MAIN);
  auto a4988Config = a4988::A4988Config(a4988::MicroStepRes::FULL);
  char *msList[] = {"F3", "F6", "F7"};
  auto stepperDriver = a4988::StepperDriverA4988("C4", "B6", msList, "C6", "C7",
                                                 "F5", a4988Config);
  stepperDriver.enable();
  for (;;) {
    // process loop
  }
  return 0;
}
