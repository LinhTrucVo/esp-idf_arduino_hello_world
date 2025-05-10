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


## Additional information:
1. Why "PRIV_REQUIRES spi_flash" cause the error Arduino.h file not found?
GitHub Copilot:
The PRIV_REQUIRES spi_flash line in your CMakeLists.txt specifies that the spi_flash component is a private dependency for your project. This means that only the current component can use the spi_flash library, and its include paths or other properties are not propagated to other components.
If your code depends on Arduino.h, it likely requires the arduino component (or a similar component that provides Arduino.h). However, since PRIV_REQUIRES only includes spi_flash as a dependency, the arduino component is not included, and the compiler cannot find Arduino.h.
Solution:
You need to add the appropriate component that provides Arduino.h to your CMakeLists.txt. For example, if the arduino component provides Arduino.h, you can add it to the REQUIRES or PRIV_REQUIRES list.
Updated CMakeLists.txt:
PRIV_REQUIRES spi_flash arduino
Explanation:
•	Adding arduino ensures that its include paths and other properties are available to your project.
•	Use REQUIRES instead of PRIV_REQUIRES if other components in your project also need access to arduino.
If arduino is not the correct component, you need to identify the component that provides Arduino.h and include it in your dependencies.

2. Why removing "PRIV_REQUIRES spi_flash" solve the error Arduino.h file not found?
GitHub Copilot
Removing PRIV_REQUIRES spi_flash from your CMakeLists.txt might solve the fatal error: Arduino.h: No such file or directory issue because of how dependencies are managed in ESP-IDF's build system.
Explanation:
i.	Dependency Isolation with PRIV_REQUIRES:
o	When you use PRIV_REQUIRES spi_flash, you are explicitly stating that your component only depends on spi_flash and no other components.
o	This isolates your component from other dependencies, including the one that provides Arduino.h (likely the arduino component or a similar one).
o	If Arduino.h is provided by another component, it won't be included in the build process because PRIV_REQUIRES restricts the scope of dependencies.
ii.	Removing PRIV_REQUIRES spi_flash:
o	By removing PRIV_REQUIRES spi_flash, your component is no longer restricted to just the spi_flash dependency.
o	This allows the build system to include other components (like arduino) that might be implicitly required by your code or the project.
iii.	Implicit Inclusion of arduino:
o	If your project or environment is set up to include the arduino component by default, removing PRIV_REQUIRES spi_flash allows the build system to resolve and include arduino, which provides Arduino.h.
Why This Works:
•	The ESP-IDF build system automatically resolves dependencies based on the components used in your project. By restricting dependencies with PRIV_REQUIRES, you might unintentionally exclude necessary components.
•	Removing PRIV_REQUIRES spi_flash allows the build system to include all required components, including the one that provides Arduino.h.
Recommendation:
If you need both spi_flash and arduino, explicitly include both in your CMakeLists.txt:
REQUIRES spi_flash arduino
This ensures that both components are available without restricting dependencies.

