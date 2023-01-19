#include "utils.h"

/**
 * When the laptop is plugged in, we expect that when the pin is touched, the
 * value becomes 0. but on battery power, on being touched the pin's value lies
 * somewhere between 15 to 30 depending on the individual pin and its wiring in
 * the case.
 *
 * so the idea is when user touches a pin (optional), we record the minimum we
 * get.
 *  - if the minimum is < 10, continue with the default thresholds.
 *  - But if the minimum lies between ~10/15-30, we switch to battery mode
 *    thresholds
 * @return int minimum value recorded on battery mode
 */
short detect_on_battery(const TouchKey *touch_key_arr, const short len) {
    int minimum_recorded = 100;

    for (auto i = 0; i < len; i++) {
        auto &key = touch_key_arr[i];
        auto  val = touchRead(key.pin);
        if (val < minimum_recorded) {
            minimum_recorded = val;
        }
    }

    if (15 <= minimum_recorded && minimum_recorded <= 30) {
        return minimum_recorded;
    }
    return 0;
}