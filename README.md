| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 | Linux |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | -------- | -------- | -------- | ----- |

# Hello World Example

Starts a FreeRTOS task to print "Hello World".

(See the README.md file in the upper level 'examples' directory for more information about examples.)

## Example folder contents

This is the modification of esp-idf hello_world example:<br>
The changes are:
1. Add Arduino as esp-idf's component (as git's submodule)
2. Change the target from esp32 to esp32c3
3. Enable USB CDC to Serial communication via USB on esp32c3
4. Configure the esp32c3 built-in JTAG for flashing and debugging
5. Add .devcontainer\select_idf_version.sh.sh file to quickly select the way to build using Docker

This video show the progress of creating this project:<br>
https://www.youtube.com/watch?v=Zp-bnV0JAf8

## How to use example

1. Clone this repo:<br>
```
git clone https://github.com/LinhTrucVo/esp-idf_arduino_hello_world.git
```

2. Enter the repo:<br>
```
cd esp-idf_arduino_hello_world
```

3. Update (clone) Arduino submodule:<br>
```
git submodule update --init --recursive --depth 1
```

4. Select target (esp32 | esp32c3), build, flash and monitor seial output
```
python .\script\target_setup\select_and_setting_target.py esp32
```

## All in one (clone, build, flash, monitor)
```
git clone https://github.com/LinhTrucVo/esp-idf_arduino_hello_world.git
cd esp-idf_arduino_hello_world
git submodule update --init --recursive --depth 1
python .\script\target_setup\select_and_setting_target.py esp32

```

To check out the Aduino submodule latest commit (optionals)
```
cd ./components/arduino
git fetch origin master --depth 1
git checkout origin/master

cd ../..
git add .
git commit -m "Update arduino submodule"
```
Now the submodule is detached (commit is updated but not yet be added to git cache to track),<br>
To add the new submodule commit to git cache for tracking, <br>
Back to the main repo then "add" and "commit" again,<br>
From now on, every time executing the command to update submodule, <br>
the commit we have just updated will be used instead of the commit at the time creating submodule.<br>


Other guideline from espressif
- [ESP32 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html)
- [ESP32-S2 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/get-started/index.html)


## Build using Docker:

This repo provides 2 option to build the Docker Image, run the **select_idf_version.sh.sh** file to select the coresponding option.
1. Using the image with **latest** esp-idf version
2. Using the image with **specific** esp-idf version
<br>

After select the option, follow the following steps:

### If using latest version:<br>
**Build Docker** image first (build the image one time is enough), the image name is specified after the -t:
```
docker build -t esp_idf:latest ./.devcontainer
```

**Build SW**:<br>
Windows
```
docker run --rm -v %cd%:/project -w /project -e HOME=/tmp esp_idf:latest idf.py build
```
Linux
```
docker run --rm -v $PWD:/project -w /project -e HOME=/tmp esp_idf:latest idf.py build
```


### If using specific version:<br>
**Build Docker** image first (build the image one time is enough), the image name is specified after the -t:
```
docker build --build-arg IDF_CLONE_BRANCH_OR_TAG=v5.4.1 -t esp_idf:v5.4.1 ./.devcontainer
```

**Build SW**:<br>
Windows
```
docker run --rm -v %cd%:/project -w /project -e HOME=/tmp esp_idf:v5.4.1 idf.py build
```
Linux
```
docker run --rm -v $PWD:/project -w /project -e HOME=/tmp esp_idf:v5.4.1 idf.py build
```

<br>Copy this to git bash to perform the steps from Repo Cloning to build:
```
# Clone this repo:
git clone https://github.com/LinhTrucVo/esp-idf_arduino_hello_world.git

# Enter the repo directory:
cd esp-idf_arduino_hello_world

# Initialize and update submodules:
git submodule update --init --recursive --depth 1

# Go to the .devcontainer directory:
cd ./.devcontainer

# Select Docker image build option (2 = specific esp-idf version):
./select_idf_version.sh.sh 2

# Back to the parent directory
cd ..

# Build the Docker image with a specific esp-idf version:
docker build --build-arg IDF_CLONE_BRANCH_OR_TAG=v5.4.1 -t esp_idf:v5.4.1 ./.devcontainer

# Build the project inside the Docker container:
MSYS_NO_PATHCONV=1 docker run --rm -v $PWD:/project -w /project -e HOME=/tmp esp_idf:v5.4.1 idf.py build
```

## ESP32 CYD TFT Touch Screen setup:
```
git clone https://github.com/LinhTrucVo/esp-idf_arduino_hello_world.git
cd esp-idf_arduino_hello_world
git submodule update --init --recursive --depth 1
python .\script\target_setup\select_and_setting_target.py esp32
python .\script\esp32_cyd_tft_touch_screen\setup_cyd_lvgl.py

```

## Additional information:
1. Why "PRIV_REQUIRES spi_flash" cause the error Arduino.h file not found?<br><br>
GitHub Copilot:<br>
The PRIV_REQUIRES spi_flash line in your CMakeLists.txt specifies that the spi_flash component is a private dependency for your project. This means that only the current component can use the spi_flash library, and its include paths or other properties are not propagated to other components.<br>
If your code depends on Arduino.h, it likely requires the arduino component (or a similar component that provides Arduino.h).<br>However, since PRIV_REQUIRES only includes spi_flash as a dependency, the arduino component is not included, and the compiler cannot find Arduino.h.<br>
**Solution**:<br>
You need to add the appropriate component that provides Arduino.h to your CMakeLists.txt. For example, if the arduino component provides Arduino.h, you can add it to the REQUIRES or PRIV_REQUIRES list.<br>
Updated CMakeLists.txt:<br>
PRIV_REQUIRES spi_flash arduino<br>
Explanation:<br>
•	Adding arduino ensures that its include paths and other properties are available to your project.<br>
•	Use REQUIRES instead of PRIV_REQUIRES if other components in your project also need access to arduino.<br>
If arduino is not the correct component, you need to identify the component that provides Arduino.h and include it in your dependencies.<br><br>

2. Why removing "PRIV_REQUIRES spi_flash" solve the error Arduino.h file not found?<br><br>
GitHub Copilot<br>
Removing PRIV_REQUIRES spi_flash from your CMakeLists.txt might solve the fatal error: Arduino.h: No such file or directory issue because of how dependencies are managed in ESP-IDF's build system.<br>
Explanation:<br>
i.	Dependency Isolation with PRIV_REQUIRES:<br>
o	When you use PRIV_REQUIRES spi_flash, you are explicitly stating that your component only depends on spi_flash and no other components.<br>
o	This isolates your component from other dependencies, including the one that provides Arduino.h (likely the arduino component or a similar one).<br>
o	If Arduino.h is provided by another component, it won't be included in the build process because PRIV_REQUIRES restricts the scope of dependencies.<br>
ii.	Removing PRIV_REQUIRES spi_flash:<br>
o	By removing PRIV_REQUIRES spi_flash, your component is no longer restricted to just the spi_flash dependency.<br>
o	This allows the build system to include other components (like arduino) that might be implicitly required by your code or the project.
iii.	Implicit Inclusion of arduino:<br>
o	If your project or environment is set up to include the arduino component by default, removing PRIV_REQUIRES spi_flash allows the build system to resolve and include arduino, which provides Arduino.h.
Why This Works:<br>
•	The ESP-IDF build system automatically resolves dependencies based on the components used in your project. By restricting dependencies with PRIV_REQUIRES, you might unintentionally exclude necessary components.<br>
•	Removing PRIV_REQUIRES spi_flash allows the build system to include all required components, including the one that provides Arduino.h.
Recommendation:<br>
If you need both spi_flash and arduino, explicitly include both in your CMakeLists.txt:<br>
REQUIRES spi_flash arduino<br>
This ensures that both components are available without restricting dependencies.

