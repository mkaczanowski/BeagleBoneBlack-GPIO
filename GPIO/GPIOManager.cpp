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

#include "./GPIOManager.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <fstream>
#include <algorithm>

#include "./GPIOConst.h"

namespace GPIO {

/*
 * GPIOManager instance
 */
GPIOManager* GPIOManager::instance = NULL;

/**
 * Returns pointer to GPIOManager singleton instance
 */

GPIOManager* GPIOManager::getInstance() {
  if (instance == NULL) {

    instance = new GPIOManager();
  }

  return instance;
}

/**
 * Constructor
 */
GPIOManager::GPIOManager() {
}

/**
 * On destruct clean all exported pins
 */
GPIOManager::~GPIOManager() {
  this->clean();
}

/**
 * Export pin (equivalent to i.e echo "68" > /sys/class/gpio/export)
 */

int GPIOManager::exportPin(unsigned int gpio) {
  std::ofstream stream(SYSFS_GPIO_DIR "/export");

  if (!stream) {
    fprintf(stderr, "OPERATION FAILED: Unable to export GPIO no. %u key: %s",
            gpio, GPIOConst::getInstance()->getGpioKeyByPin(gpio));
    return -1;
  }

  stream << gpio;
  stream.close();

  this->exportedPins.push_back(gpio);

  return 0;
}

/**
 * Unexport pin (equivalent to i.e echo "68" > /sys/class/gpio/unexport)
 */
int GPIOManager::unexportPin(unsigned int gpio) {
  std::ofstream stream(SYSFS_GPIO_DIR "/unexport");

  if (!stream) {
    fprintf(stderr, "OPERATION FAILED: Unable to unexport GPIO no. %u key: %s",
            gpio, GPIOConst::getInstance()->getGpioKeyByPin(gpio));
    return -1;
  }

  stream << gpio;
  stream.close();

  // Remove element from the list
  this->exportedPins.erase(
      std::remove(this->exportedPins.begin(), this->exportedPins.end(), gpio),
      this->exportedPins.end());

  return 0;
}

/**
 * Set direction (equivalent to i.e echo "in" > /sys/class/gpio68/direction)
 */
int GPIOManager::setDirection(unsigned int gpio, DIRECTION direction) {
  char path[50];
  snprintf(path, sizeof(path), SYSFS_GPIO_DIR "/gpio%u/direction", gpio);

  std::ofstream stream(path);
  if (!stream) {
    fprintf(stderr,
            "OPERATION FAILED: Unable to set direction GPIO no. %u key: %s",
            gpio, GPIOConst::getInstance()->getGpioKeyByPin(gpio));
    return -1;
  }

  if (direction == OUTPUT) {
    stream << "out";
  } else {
    stream << "in";
  }

  stream.close();

  return 0;
}

/**
 * Get direction (equivalent to i.e cat /sys/class/gpio68/direction)
 */
int GPIOManager::getDirection(unsigned int gpio) {
  char path[50], direction;
  snprintf(path, sizeof(path), SYSFS_GPIO_DIR "/gpio%u/direction", gpio);

  std::ifstream stream(path);
  if (!stream) {
    fprintf(stderr,
            "OPERATION FAILED: Unable to get direction GPIO no. %u key: %s",
            gpio, GPIOConst::getInstance()->getGpioKeyByPin(gpio));
    return -1;
  }

  stream >> direction;

  stream.close();

  return (direction == 'i') ? INPUT : OUTPUT;
}

/**
 * Set value (equivalent to i.e echo "1" > /sys/class/gpio68/value)
 */
int GPIOManager::setValue(unsigned int gpio, PIN_VALUE value) {
  char path[50];
  snprintf(path, sizeof(path), SYSFS_GPIO_DIR "/gpio%u/value", gpio);

  std::ofstream stream(path);
  if (!stream) {
    fprintf(stderr, "OPERATION FAILED: Unable to set value GPIO no. %u key: %s",
            gpio, GPIOConst::getInstance()->getGpioKeyByPin(gpio));
    return -1;
  }

  stream << value;
  stream.close();

  return 0;
}

/**
 * Get value (equivalent to i.e cat /sys/class/gpio68/value)
 */
int GPIOManager::getValue(unsigned int gpio) {
  char path[50], value;
  snprintf(path, sizeof(path), SYSFS_GPIO_DIR "/gpio%u/value", gpio);

  std::ifstream stream(path);
  if (!stream) {
    fprintf(stderr, "OPERATION FAILED: Unable to get value GPIO no. %u key: %s",
            gpio, GPIOConst::getInstance()->getGpioKeyByPin(gpio));
    return -1;
  }

  stream >> value;
  stream.close();

  return (value == '1') ? HIGH : LOW;
}

/**
 * Set edge (equivalent to i.e echo "rising" > /sys/class/gpio68/edge)
 */
int GPIOManager::setEdge(unsigned int gpio, EDGE_VALUE value) {
  char path[50];
  snprintf(path, sizeof(path), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

  std::ofstream stream(path);
  if (!stream) {
    fprintf(stderr, "OPERATION FAILED: Unable to set edge GPIO no. %d key: %s",
            gpio, GPIOConst::getInstance()->getGpioKeyByPin(gpio));
    return -1;
  }

  stream << GPIOConst::getInstance()->getEdgeValueByIndex(value);
  stream.close();

  return 0;
}

/**
 * Get edge (equivalent to i.e cat /sys/class/gpio68/edge)
 */
int GPIOManager::getEdge(unsigned int gpio) {
  char path[50], value[7];
  snprintf(path, sizeof(path), SYSFS_GPIO_DIR "/gpio%u/edge", gpio);

  std::ifstream stream(path);
  if (!stream) {
    fprintf(stderr, "OPERATION FAILED: Unable to get value GPIO no. %u key: %s",
            gpio, GPIOConst::getInstance()->getGpioKeyByPin(gpio));
    return -1;
  }

  stream >> value;
  stream.close();

  return GPIOConst::getInstance()->getEdgeIndexByValue(value);
}

/**
 * Wait for edge event
 */
int GPIOManager::waitForEdge(unsigned int gpio, EDGE_VALUE value) {
  char path[50], buf;
  int efd, fd;
  struct epoll_event events, ev;

  snprintf(path, sizeof(path), SYSFS_GPIO_DIR "/gpio%u/value", gpio);

  //The following lines were commented out for more efficient polling
  //this->exportPin(gpio);
  //this->setDirection(gpio, INPUT);
  //this->setEdge(gpio, value);

  // Get value file descriptor
  fd = open(path, O_RDONLY | O_NONBLOCK);

  // Create epoll instance
  efd = epoll_create(1);

  // Fill the event structure and register
  ev.data.fd = fd;
  ev.events = EPOLLIN | EPOLLET | EPOLLPRI;

  epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);

  // Ignore the first read (initial value)
  for (int i = 0; i < 2; i++) {
    if ((epoll_wait(efd, &events, 1, -1)) == -1) {
      return -1;
    }
  }

  lseek(events.data.fd, 0, SEEK_SET);
  if (read(events.data.fd, &buf, sizeof(buf)) != 1) {
    return -1;
  }

  close(efd);
  close(fd);

  return (buf & 1);
}

/**
 * Count already exported pins
 */
int GPIOManager::countExportedPins() {
  return this->exportedPins.size();
}

/**
 * Unexport all of already exported pins
 */
void GPIOManager::clean() {
  for (int i = 0; i < static_cast<int>(this->exportedPins.size()); i++) {
    this->unexportPin(this->exportedPins[i]);
  }
}

} /* namespace GPIO */
