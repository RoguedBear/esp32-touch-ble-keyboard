#include "CircularBuffer.h"
#include "KEY_CONFIG.h"
#include "TouchKey.h"

#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard                           bleKeyboard;
CircularBuffer<key_press_timestamp_t> buffer(5);

void test_ISR_handler_arg(void *obj) {
    TouchKey *class_ptr = (TouchKey *)obj;
    buffer.push(class_ptr->generate_timestamp());
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
                                (void *)key_obj, 40);
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
        delay(10);
        digitalWrite(2, LOW);
        delay(500);
    }
    // TODO: clear the buffer here
}

void loop() {
    if (bleKeyboard.isConnected()) {
        digitalWrite(2, LOW);
        if (!buffer.is_empty()) {
            key_press_timestamp_t timestamp      = buffer.pop();
            TouchKey *            key_obj        = timestamp.obj;
            unsigned long         old_last_touch = key_obj->last_touch_ms;
            if (key_obj->process_touch(timestamp)) {
                bleKeyboard.write(key_obj->letter_to_press);

                Serial.printf(
                    "Timestamp: letter:%d|%c last_recorded_ms:%lu \tmillis: "
                    "%lu \tdelta new:%lu\n\n",
                    key_obj->letter_to_press, key_obj->letter_to_press,
                    timestamp.last_touch_ms, timestamp.time_recorded_ms,

                    timestamp.time_recorded_ms - old_last_touch);
            }
        }

    } else {
        digitalWrite(2, HIGH);
        // TODO: clear the buffer here
    }

    delay(50);
}
int main() {}