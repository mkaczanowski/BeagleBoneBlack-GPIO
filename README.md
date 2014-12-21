BeagleBoneBlack-GPIO
====================

Simple C++ library that handles GPIO calls for BeagleBone Black

### Features and methods
1.  Easy operations on gpio names:
 - GPIOConst::getGpioByKey - i.e "P8_10"
 - GPIOConst::getGpioByName - i.e "GPIO0_7"
 - GPIOConst::getGpioNameByPin - i.e "12"
 - GPIOConst::getGpioKeyByPin - i.e "12"
 - GPIOConst::isPinAllocatedByDefault - refering to https://github.com/derekmolloy/boneDeviceTree/tree/master/docs

2. Easy operations on gpio sysfs descriptors:
 - GPIOManager::exportPin - Export pin (equivalent to i.e echo "68" > /sys/class/gpio/export)
 - GPIOManager::unexportPin - Unexport pin (equivalent to i.e echo "68" > /sys/class/gpio/unexport)
 - GPIOManager::setDirection - Set direction (equivalent to i.e echo "in" > /sys/class/gpio/gpio68/direction)
 - GPIOManager::getDirection - Get direction (equivalent to i.e cat /sys/class/gpio/gpio68/direction)
 - GPIOManager::setValue - Set value (equivalent to i.e echo "1" > /sys/class/gpio/gpio68/value)
 - GPIOManager::getValue - Get value (equivalent to i.e cat /sys/class/gpio/gpio68/value)
 - GPIOManager::setEdge - Set edge (equivalent to i.e echo "rising" > /sys/class/gpio/gpio68/edge)
 - GPIOManager::getEdge - Get edge (equivalent to i.e cat /sys/class/gpio/gpio68/edge)
 - GPIOManager::waitForEdge - Wait for edge event
 - GPIOManager::countExportedPins - Count already exported pins
 - GPIOManager::clean - Unexport all of already exported pins
 
3. Export/unexport sanity check
4. Table with pins being exported by default

### Compilation
g++ GPIO/GPIOConst.cpp GPIO/GPIOManager.cpp BeagleBoneBlack-GPIO.cpp -o example
