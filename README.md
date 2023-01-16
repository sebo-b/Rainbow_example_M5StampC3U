# A simple RGB led example on M5Stamp C3U (esp32c3)

This is a simple example of generating rainbow colors on esp32 with RGB led.

It touches on the following areas:
- usage of led_strip component,
- usage of hardware interruptions,
- debouncing button press in ISR
- usage of RTOS tasks and task notifications
- pretty neat RGB rainbow generation without additional state storage

## What is needed
- Any ESP32 module with an RGB led
    - I used M5Stamp C3U
    - For other modules, it will be needed to change GPIO of the button and the led (`LED_GPIO` & `BTN_GPIO` in `rainbow_example.c`)
    - For other than ESP32C3 modules, it will be needed to reset the target via `idf.py set-target XXX`
- Installed and configured IDF (standalone or as a plugin to IDE)

## How to build and install

This may be enough to build and flash (read the official IDF documentation for more details):
```
idf.py build
cd build
ninja flash
```
