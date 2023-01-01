#include "TouchKey.h"

TouchKey::TouchKey(uint8_t pin, char letter_to_press, int threshold)
    : pin(pin), letter_to_press(letter_to_press), is_pressed(false),
      threshold(threshold) {}

key_press_timestamp_t TouchKey::generate_timestamp() {
    key_press_timestamp_t timestamp = {
        this,          // obj
        last_touch_ms, // last_touch_ms
        millis()       // current_time
    };

    return timestamp;
}

bool TouchKey::process_touch(key_press_timestamp_t &timestamp) {
    /**
     * Basic keyboard key press detection.
     * if there's a sufficient delay b/w the net key press & the last recorded
     * one, that is a key press
     */
    if (timestamp.time_recorded_ms - this->last_touch_ms >=
        KEY_PRESS_TIMEOUT_ms) {
        this->last_touch_ms = timestamp.time_recorded_ms;
        return true;
    } else {
        return false;
    }
}