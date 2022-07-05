/**
 * basic harcoded code, POC that this technique works
 */
#include "CircularBuffer.h"
#include "TouchKey.h"
// #define USE_NIMBLE

#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard                           bleKeyboard;
CircularBuffer<key_press_timestamp_t> buffer(64);
TouchKey                              test_key(T3, 'w');

volatile unsigned long last_touch = 0;
volatile char *        test;

// class TestClass {
//   public:
//     char c;
//     TestClass(char chr) : c(chr) {}
// };
// TestClass w('w');
// void test_ISR_handler() {
//     if (millis() - last_touch >= 100) {
//         last_touch = millis();
//         buffer.push('w');
//     }
// }

void test_ISR_handler_arg(void *obj) {
    TouchKey *class_ptr = (TouchKey *)obj;
    buffer.push(class_ptr->generate_timestamp());
}

void setup() {
    // TODO: refactor all this
    pinMode(2, OUTPUT);
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    touchAttachInterruptArg(T3, test_ISR_handler_arg, (void *)&test_key, 40);
    bleKeyboard.begin();
    while (!bleKeyboard.isConnected()) {
        digitalWrite(2, HIGH);
        delay(10);
        digitalWrite(2, LOW);
        delay(500);
    }

    // touchAttachInterrupt(T3, test_ISR_handler, 60);
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
                    "Timestamp: last_recorded_ms:%lu \tmillis: %lu "
                    "\tdelta:%lu \tdelta new:%lu\n\n",
                    timestamp.last_touch_ms, timestamp.time_recorded_ms,
                    timestamp.time_recorded_ms - timestamp.last_touch_ms,
                    timestamp.time_recorded_ms - old_last_touch);
            }
        }

    } else {
        // Serial.println("Not connected :/");
        digitalWrite(2, HIGH);
        // TODO: clear the buffer here
    }

    // Serial.println("Waiting 5 seconds...");
    delay(50);
}
int main() {}