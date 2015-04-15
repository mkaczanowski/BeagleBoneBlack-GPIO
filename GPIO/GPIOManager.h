/*
 * Copyright (c) 2013 Mateusz Kaczanowski
 * www.mkaczanowski.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef BEAGLEBONEBLACK_GPIO_SRC_GPIO_GPIOMANAGER_H_
#define BEAGLEBONEBLACK_GPIO_SRC_GPIO_GPIOMANAGER_H_


#ifdef QBEAGLEBONEBLACKGPIO_LIBRARY
    #include <qbeagleboneblackgpio_global.h>
#endif


#define SYSFS_GPIO_DIR "/sys/class/gpio"
#include <vector>

namespace GPIO {

enum DIRECTION {
  INPUT = 0,
  OUTPUT = 1
};

enum PIN_VALUE {
  LOW = 0,
  HIGH = 1
};

enum EDGE_VALUE {
  NONE = 0,
  RISING = 1,
  FALLING = 2,
  BOTH = 3
};

class
        #ifdef QBEAGLEBONEBLACKGPIO_LIBRARY
        QBEAGLEBONEBLACKGPIOSHARED_EXPORT
        #endif

        GPIOManager {
 public:
  GPIOManager();
  virtual ~GPIOManager();

  static GPIOManager* getInstance();
  int exportPin(unsigned int gpio);
  int unexportPin(unsigned int gpio);
  int setDirection(unsigned int gpio, DIRECTION direction);
  int getDirection(unsigned int gpio);
  int setValue(unsigned int gpio, PIN_VALUE value);
  int getValue(unsigned int gpio);
  int setEdge(unsigned int gpio, EDGE_VALUE value);
  int getEdge(unsigned int gpio);
  int waitForEdge(unsigned int gpio, EDGE_VALUE value);
  int countExportedPins();
  void clean();
 private:
  static GPIOManager* instance;
  std::vector<unsigned int> exportedPins;
};

} /* namespace GPIO */
#endif  // BEAGLEBONEBLACK_GPIO_SRC_GPIO_GPIOMANAGER_H_
