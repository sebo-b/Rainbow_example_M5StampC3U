# A simple RGB led example on M5Stamp C3U (esp32c3)

This is a simple example of generating rainbow colors on esp32 with RGB led.

It touches of the following areas:
- usage of led_strip component
- usage of hardware interuptions
- debouncing btn press in interruption
- usage of RTOS tasks and task notifications
- pretty neat RGB rainbow generation without additional state storage

## What is needed
- Any ESP32 module with a RGB led
    - I used M5Stamp C3U
    - For other modules it will be needed to change GPIOs of a button and led (`LED_GPIO` & `BTN_GPIO` in `rainbow_example.c`)
    - For othr than ESP32C3 modules it will be needed to reset target via `idf.py set-target XXX`
- Installed and configured IDF (standalone or as plugin to IDE)

## How to build and install

This may be enough to (read official IDF documentation for more details):
```
idf.py build
cd build
ninja flash
```