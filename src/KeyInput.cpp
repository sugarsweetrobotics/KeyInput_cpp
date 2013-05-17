// -*- C++ -*-
/*!
 * @file  KeyInput.cpp
 * @brief Key Input RTC
 * @date $Date$
 *
 * $Id$
 */

#include "KeyInput.h"


enum {
    UP = 65536,
    DOWN,
    LEFT,
    RIGHT,
    SPACE,
    ESCAPE,
};


#ifdef __cplusplus

#endif

#ifdef WIN32
#include <conio.h>
#else
#include <termios.h>
        struct termios m_oldTermios;
#endif


static void init_scr() {
#ifdef WIN32
	system("cls");
#else
  	struct termios myTermios;
	tcgetattr(fileno(stdin), &m_oldTermios);
	tcgetattr(fileno(stdin), &myTermios);
    
	myTermios.c_cc[VTIME] = 0;
#ifdef linux
	myTermios.c_cc[VMIN] = 0;
#else // MacOS
	myTermios.c_cc[VMIN] = 1;
#endif
	myTermios.c_lflag &= (~ECHO & ~ICANON);
	tcsetattr(fileno(stdin), TCSANOW, &myTermios);
        
#endif
}

static void clear_scr() {
#ifdef WIN32
	system("cls");
#else
    system("clear");
#endif
}

static void exit_scr() {
#ifdef WIN32
	system("cls");
#else
	system("reset");
#endif
}

static int myKbhit() {
#ifdef WIN32
  return _kbhit();
#else
  fd_set fdset;
  struct timeval timeout;
  FD_ZERO( &fdset ); 
  FD_SET( 0, &fdset );
  timeout.tv_sec = 0; 
  timeout.tv_usec = 0;
  int ret = select(0+1 , &fdset , NULL , NULL , &timeout );
  return ret;
#endif
}


static int myGetch() {
#ifdef WIN32
	return _getch();
#else
    int keys[5] = {-1, -1, -1, -1, -1};
    int key = getchar();
	switch(key) {
        case -1:
        case 0:
            return -1;
            
        case ' ':
            return SPACE;
        case 27:
            key = getchar();
            switch(key) {
                case -1:
                    return ESCAPE;
                case 79:
                    key = getchar();
                    return key;
                case '[':
                    for(int i = 0;i < 5;i++) {
                        if(key == -1 || key == 27) break;
                        keys[i] = key = getchar();
                    }
                    ungetc(key, stdin);
                    
                    switch(keys[0]) {
                        case 65: return UP;
                        case 66: return DOWN;
                        case 67: return RIGHT;
                        case 68: return LEFT;
                        default: return keys[0];
                    }
            }
	}
	return key;
#endif
}


#ifdef __cplusplus

#endif


// Module specification
// <rtc-template block="module_spec">
static const char* keyinput_spec[] =
  {
    "implementation_id", "KeyInput",
    "type_name",         "KeyInput",
    "description",       "Key Input RTC",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "Tutorial",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",
    // Widget
    "conf.__widget__.debug", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
KeyInput::KeyInput(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_keysOut("keys", m_keys),
    m_functionkeysOut("functionkeys", m_functionkeys)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
KeyInput::~KeyInput()
{
}



RTC::ReturnCode_t KeyInput::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("keys", m_keysOut);
  addOutPort("functionkeys", m_functionkeysOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  // </rtc-template>

  init_scr();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t KeyInput::onFinalize()
{
  clear_scr();
  exit_scr();
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t KeyInput::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KeyInput::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t KeyInput::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t KeyInput::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t KeyInput::onExecute(RTC::UniqueId ec_id)
{
  if (myKbhit()) {
    int c = myGetch();
    if (c == SPACE) {
      m_functionkeys.data = "space";
      m_functionkeysOut.write();
    } else if(c == UP) {
      m_functionkeys.data = "up";
      m_functionkeysOut.write();
    } else if(c == DOWN) {
      m_functionkeys.data = "down";
      m_functionkeysOut.write();
    } else if(c == LEFT) {
      m_functionkeys.data = "left";
      m_functionkeysOut.write();
    } else if(c == RIGHT) {
      m_functionkeys.data = "right";
      m_functionkeysOut.write();
    } else {
      m_keys.data = (char)c;
      m_keysOut.write();
    }
  }
    
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t KeyInput::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KeyInput::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KeyInput::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KeyInput::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t KeyInput::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void KeyInputInit(RTC::Manager* manager)
  {
    coil::Properties profile(keyinput_spec);
    manager->registerFactory(profile,
                             RTC::Create<KeyInput>,
                             RTC::Delete<KeyInput>);
  }
  
};


