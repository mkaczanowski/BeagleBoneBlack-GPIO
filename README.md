[![Build Status][travis-badge]][travis]

[travis-badge]: https://travis-ci.org/mkaczanowski/BeagleBoneBlack-GPIO.svg?branch=master
[travis]: https://travis-ci.org/mkaczanowski/BeagleBoneBlack-GPIO/

# BeagleBoneBlack-GPIO

The library is a convenient manager for BeagleBone Black GPIO pins. In a nutshell it:

* wraps sysfs operations (i.e., export, unexport, read, write, wait)
* translates pin IDs to pin header names (ie. GPIO 38 = P8_03)
* handles the state (i.e., unexports previously exported pins)
* provides detailed data about the selected pin (i.e., allocation, location, id, modes, ...)

## Example
```
#compiled under BeagleBone(target) environment
#if compiled under VM(host) environment, changed "g++" to "g++-arm-linux-gnueabihf" to cross compile
g++ -std=c++17 -lstdc++fs gpio/*.cpp gpio/*.h main.cpp -o example
```

```
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
```

## Reference
### Pinout
![](http://mkaczanowski.com/wp-content/uploads/2014/03/resizedimage600667-hwio-beaglebone-ports2.png) 

### Pin setup (by [Derek Molloy](https://github.com/derekmolloy/boneDeviceTree))
[![](http://mkaczanowski.com/wp-content/uploads/2014/03/p8p9-300x169.jpg) ](http://mkaczanowski.com/wp-content/uploads/2014/03/BeagleboneBlackP8HeaderTable.pdf)  [![](http://mkaczanowski.com/wp-content/uploads/2014/03/P9-300x146.jpg)](http://mkaczanowski.com/wp-content/uploads/2014/03/BeagleboneBlackP9HeaderTable.pdf)
