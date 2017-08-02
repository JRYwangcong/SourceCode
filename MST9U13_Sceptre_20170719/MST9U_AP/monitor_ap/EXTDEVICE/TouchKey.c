
#include "Global.h"

#if ENABLE_TOUCH_KEY

#if TOUCH_KEY_SOURCE == TOUCH_KEY_SOURCE_ITE
#include "IT7230_TouchKey.c"
#include "IT7230_I2C.c"
#elif TOUCH_KEY_SOURCE == TOUCH_KEY_SOURCE_SMSC

#elif TOUCH_KEY_SOURCE == TOUCH_KEY_SOURCE_CYPRESS

#else
#warning "please implement function for new touch key source"
#endif
#endif

