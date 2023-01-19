#ifndef __util_h
#define __util_h

#include "CONFIG.h"
#include "TouchKey.h"

/**
 * Includes utility functions that help refactor main.cpp code
 */

short detect_on_battery(const TouchKey *touch_key_arr, short len);

#if OVERRIDE_15_CHAR_LIMIT == true
// Modify this array to add custom emoticons
static String suffix_arr[] = {":)",  ":o", ":p", "o_O", "O_o", "O_O",
                              "^_^", "",   "~~", "uWu", "UwU"};
#endif
String generate_suffix(String name);

#endif