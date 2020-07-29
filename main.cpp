#include "gpio/const.h"
#include "gpio/manager.h"

using namespace beagle::gpio;

int main() {
  GPIOManager *gp = GPIOManager::getInstance();
  auto pin = getPin("P8_10");

  if (!pin) {
    return 255;
  }

  gp->exportPin(pin.value().gpio);
  gp->setDirection(pin.value().gpio, INPUT);
  gp->setEdge(pin.value().gpio, RISING);

  gp->waitForEdge(pin.value().gpio);
  gp->~GPIOManager();

  return 0;
}
