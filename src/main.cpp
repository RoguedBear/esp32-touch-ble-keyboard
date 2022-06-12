/**
 * basic harcoded code, POC that this technique works
 */
#include "CircularBuffer.h"
// #define USE_NIMBLE

#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard          bleKeyboard;
CircularBuffer<char> buffer(64);

volatile unsigned long last_touch = 0;
volatile char *        test;

class TestClass {
  public:
    char c;
    TestClass(char chr) : c(chr) {}
};

TestClass w('w');

void test_ISR_handler() {
    if (millis() - last_touch >= 50) {
        last_touch = millis();
        buffer.push('w');
    }
}

void test_ISR_handler_arg(void *letter) {
    if (millis() - last_touch >= 50) {
        last_touch           = millis();
        TestClass *class_ptr = (TestClass *)letter;
        test                 = &class_ptr->c;
        buffer.push(class_ptr->c);
    }
}

void setup() {
    // TODO: refactor all this
    pinMode(2, OUTPUT);
    Serial.begin(115200);
    Serial.println("Starting BLE work!");
    Serial.println(w.c);
    touchAttachInterruptArg(T3, test_ISR_handler_arg, (void *)&w, 40);
    bleKeyboard.begin();
    while (!bleKeyboard.isConnected()) {
        digitalWrite(2, HIGH);
        delay(10);
        digitalWrite(2, LOW);
        delay(500);
    }

    // touchAttachInterrupt(T3, test_ISR_handler, 60);
}

void loop() {
    if (bleKeyboard.isConnected()) {
        digitalWrite(2, LOW);
        if (!buffer.is_empty()) {
            char stuff = buffer.pop();
            Serial.printf("buffer=%d, ptr=%d, ptr_addr=%p\n", (int)stuff, *test,
                          test);
            bleKeyboard.write(stuff);
        }

    } else {
        // Serial.println("Not connected :/");
        digitalWrite(2, HIGH);
    }

    // Serial.println("Waiting 5 seconds...");
    delay(50);
}
int main() {}