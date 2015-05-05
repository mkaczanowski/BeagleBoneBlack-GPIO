Qt shared lib that implements BeagleBoneBlack-GPIO

Install:
  Use qmake or open QtCreator and compile the library
  
Usage:

  -Add the library to the project (.pro)
  
      LIBS += -Lpath-to-library/release -lQBeagleBoneBlackGPIO

      INCLUDEPATH += path-to-library/
      
  -using the library:
  
    #include "GPIO/GPIOManager.h"
    #include "GPIO/GPIOConst.h"
    
    GPIOManager* m_gpiomanager=GPIOManager::getInstance();

    int m_LedsPin = GPIOConst::getInstance()->getGpioByKey("P8_13");

    m_gpiomanager->exportPin(m_LedsPin);
    m_gpiomanager->setDirection(m_LedsPin,GPIO::OUTPUT);
    m_gpiomanager->setValue(m_LedsPin,GPIO::LOW);
  
