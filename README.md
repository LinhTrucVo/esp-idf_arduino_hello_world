| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 | Linux |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | -------- | -------- | -------- | ----- |

# Hello World Example

Starts a FreeRTOS task to print "Hello World".

(See the README.md file in the upper level 'examples' directory for more information about examples.)

## How to use example

1. Clone this repo:
**git clone https://github.com/LinhTrucVo/esp-idf_arduino_hello_world.git**

2. Update (clone) Arduino submodule:
**git submodule update --init --recursive --depth 1**

Other guideline from espressif
- [ESP32 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html)
- [ESP32-S2 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/get-started/index.html)


## Example folder contents

This is the modification of esp-idf hello_world example:
The change:
1. Add Arduino as esp-idf's component (as git's submodule)
2. Change the target from esp32 to esp32c3
3. Enable USB CDC to Serial communication via USB on esp32c3
4. Configure the esp32c3 built-in JTAG for flashing and debugging.

This video show the progress of creating this project:
https://www.youtube.com/watch?v=Zp-bnV0JAf8