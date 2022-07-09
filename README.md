# esp32-touch-ble-keyboard

> A configurable bluetooth keyboard that utilizes the touch sensors on ESP32 as
> keyboard keys to send keystrokes to the connected device

https://user-images.githubusercontent.com/39442192/178109969-6f2e205e-52a7-4c5b-969b-fc063abdd287.mp4

<details>
  <summary><h2>why???????</h2></summary>
  
  
Since the past few months I've been noticing that when travelling with my laptop
some keys (`2`, `w`, `s`, `x`, `f5-8`) were just not responding on the hardware
level. Smacking the keys multiple times seemed to bring them back to life until
i travelled with my laptop again. \
But as time passed, even that method to beat life into my keys became a lost hope.

I didn't feel like spending on a new keyboard and I had an esp lying around,
so......

</details>

## How to use

### Pre-requisites

open the project in VSCode and install platformio extension

### Keys layout

- define the pins and their corresponding letters you want to use in the file:
  [`includes/KEY_CONFIG.h`](https://github.com/RoguedBear/esp32-touch-ble-keyboard/blob/main/include/KEY_CONFIG.h#L4-L5)

- inside the `TOUCH_KEYS` array, initialise your keys using the format:
  `TouchKey(<<pin number>>, '<<letter to press>>')` (refer sample config in that
  file)

for the pin number, make sure to refer the pinout diagram of your ESP board and
select the touch pins only (touch pins are prefixed with `T`)

### Bluetooth device properties

- you can edit the device's bluetooth name, manufacturer's name & the battery
  level in [`include/CONFIG.h`](/include/CONFIG.h)

after you're done configuring, build the code and upload to your board

## ToDo

- [ ] improve touch processing algorithm to imitate a real keyboard's
- [ ] make a 3d printed case

## Credits

This program uses the amazing
[`ESP32-BLE-Keyboard`](https://github.com/T-vK/ESP32-BLE-Keyboard) library made
by [T-vK](https://github.com/T-vK)
