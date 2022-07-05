#ifndef __TOUCH_KEY
#define __TOUCH_KEY

/* For Testing purposes only */
#ifdef NATIVE_TEST
typedef unsigned char uint8_t;
unsigned long         millis() { // stub
    return (unsigned long)500;
}
#else
#include <Arduino.h>
#endif

#ifndef KEY_PRESS_TIMEOUT_ms
#define KEY_PRESS_TIMEOUT_ms 500
#endif

class TouchKey; // forward declaration for struct

struct key_press_timestamp_t {
    TouchKey *    obj;
    unsigned long last_touch_ms;
    unsigned long time_recorded_ms;
};

class TouchKey {
  public:
    uint8_t pin;
    bool    is_pressed;
    char    letter_to_press;

    volatile unsigned long last_touch_ms;

    TouchKey(uint8_t pin, char letter_to_press);

    /**
     * Generates a timestamp containing neccessary info to process if
     * key should be pressed or not
     */
    key_press_timestamp_t generate_timestamp();

    /**
     * @param timestamp the timestamp struct which contains key press timestamp
     * info
     * @return boolean, true if key should be pressed, false if touch should not
     * be registered
     */
    bool process_touch(key_press_timestamp_t &timestamp);
};

#endif