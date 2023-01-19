#ifndef __CONFIG_h
#define __CONFIG_h

#define KB_BLUETOOTH_NAME              "ESP32 Keyboard" // 15 char limit
#define KB_BLUETOOTH_MANUFACTURER_NAME "CaaS Inc."
#define KB_BLUETOOTH_BATTERY_LEVEL     68

#define BUFFER_SIZE          4   // buffer size to maintain
#define KEY_PRESS_TIMEOUT_ms 400 // delay in milliseconds between key presses
#define TOUCH_THRESHOLD      20  // touch threshold used to detec touch
#define ON_BAT_THRESHOLD     30  // overrides other thresholds on battery mode

// ================== EXPERIMENTAL!!! ==================
// if you want to have the keyboard's name be greater than 15 characters, you
// have to modify the ESP32-BLE-Keyboard library and remove the part where name
// is trimmed to 15 characters. After doing that you may take advantage of the
// random emojis im appending at the end of the name
#define OVERRIDE_15_CHAR_LIMIT false

#endif