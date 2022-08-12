#ifndef __CONFIG_h
#define __CONFIG_h

#define KB_BLUETOOTH_NAME              "ESP32 Keyboard" // 15 char limit
#define KB_BLUETOOTH_MANUFACTURER_NAME "CaaS Inc."
#define KB_BLUETOOTH_BATTERY_LEVEL     68

#define BUFFER_SIZE          4   // buffer size to maintain
#define KEY_PRESS_TIMEOUT_ms 400 // delay in milliseconds between key presses
#define TOUCH_THRESHOLD      40  // touch threshold used to detec touch
#endif