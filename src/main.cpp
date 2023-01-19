#include "CONFIG.h"
#include "CircularBuffer.h"
#include "KEY_CONFIG.h"
#include "TouchKey.h"

#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard(KB_BLUETOOTH_NAME, KB_BLUETOOTH_MANUFACTURER_NAME,
                        KB_BLUETOOTH_BATTERY_LEVEL);

CircularBuffer<key_press_timestamp_t> buffer(BUFFER_SIZE);
key_press_timestamp_t                 discarded_ghost_touch;
unsigned long                         __led_last_on = 0;

void test_ISR_handler_arg(void *obj) {
    TouchKey *class_ptr = (TouchKey *)obj;
    buffer.push(class_ptr->generate_timestamp());
    // #ifdef DEBUG_CODE
    //     Serial.printf("got %c at %lu\n", class_ptr->letter_to_press,
    //     millis());
    // #endif
}

/**
 * Register ISR handlers for all the defined keys in the array
 */
void register_ISR_handlers(void) {

    // using this because array size *is* known at compile time
    int array_len = sizeof(TOUCH_KEYS) / sizeof(TouchKey);

    for (int i = 0; i < array_len; i++) {
        TouchKey *key_obj = &TOUCH_KEYS[i];
        touchAttachInterruptArg(key_obj->pin, test_ISR_handler_arg,
                                (void *)key_obj, key_obj->threshold);
        Serial.printf("Registered handler for key '%c' at pin %d\n",
                      key_obj->letter_to_press, key_obj->pin);
    }
}

void setup() {
    // TODO: refactor all this
    pinMode(2, OUTPUT);
    Serial.begin(115200);

    Serial.println("Registering touch interrupts");
    register_ISR_handlers();

    Serial.println("Starting BLE work!");
    bleKeyboard.begin();

    while (!bleKeyboard.isConnected()) {
        digitalWrite(2, HIGH);
        delay(50);
        digitalWrite(2, LOW);
        delay(500);
    }
    buffer.clear();
}

void loop() {
    if (bleKeyboard.isConnected()) {
        digitalWrite(2, LOW);
        if (!buffer.is_empty()) {
#ifdef DEBUG_CODE
            String pre = buffer.print();
#endif
            key_press_timestamp_t timestamp      = buffer.pop();
            TouchKey *            key_obj        = timestamp.obj;
            unsigned long         old_last_touch = key_obj->last_touch_ms;

            /* Ghost touch prevention workaround */
            if (!buffer.is_empty() || discarded_ghost_touch.obj) {
                key_press_timestamp_t next_timestamp;
                if (!buffer.is_empty()) {
                    next_timestamp = buffer.poll();
                }
                // if we already discarded a ghost touch, check if the pins are
                // different
                if (discarded_ghost_touch.obj &&
                    discarded_ghost_touch.obj->pin != timestamp.obj->pin) {
                    next_timestamp = discarded_ghost_touch;
                }

                unsigned long delta = next_timestamp.time_recorded_ms -
                                      timestamp.time_recorded_ms;
                Serial.printf("path delta: %lu\n", delta);
                if (delta <= 100) { // ghost touches are recorded at the exact
                                    // same millisecond
                    discarded_ghost_touch = timestamp;
                    Serial.printf("Discarded ghost touch: %c, delta:%lu, "
                                  "current, next millis:%lu, %lu\n",
                                  timestamp.obj->letter_to_press, delta,
                                  timestamp.time_recorded_ms,
                                  next_timestamp.time_recorded_ms);
#ifdef DEBUG_CODE
                    Serial.println(pre + " \t||| \t" + buffer.print());
#endif
                    return;
                }
            }
            discarded_ghost_touch.obj = NULL;

            if (key_obj->process_touch(timestamp)) {
                bleKeyboard.write(key_obj->letter_to_press);

                Serial.printf(
                    "Timestamp: letter:%d|%c last_recorded_ms:%lu \tmillis: "
                    "%lu \tdelta new:%lu\n\n",
                    key_obj->letter_to_press, key_obj->letter_to_press,
                    timestamp.last_touch_ms, timestamp.time_recorded_ms,

                    timestamp.time_recorded_ms - old_last_touch);
            }
#ifdef DEBUG_CODE
            Serial.println(pre + " \t||| \t" + buffer.print());
#endif
        }

    } else {
        unsigned long now = millis();
        if (now - __led_last_on >= 500) {
            __led_last_on = now;
            digitalWrite(2, HIGH);
            delay(50);
            digitalWrite(2, LOW);
        }

        buffer.clear();
    }

    delay(10);
}
int main() {}