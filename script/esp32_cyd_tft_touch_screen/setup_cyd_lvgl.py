"""
setup_cyd_lvgl.py

This script automates the setup of the ESP32 CYD TFT Touch Screen project with LVGL and related libraries.

Main steps performed:
- Loads ESP-IDF path from .vscode/settings.json.
- Clones required component repositories (lvgl, TFT_eSPI, XPT2046_Touchscreen) into the components directory.
- Copies a custom CMakeLists.txt into the XPT2046_Touchscreen component.
- Copies main.cpp and sdkconfig.defaults to the appropriate project locations.
- Copies the ui folder into components/ui.
- Generates and runs a batch file to clone the repositories.
- Generates a batch file to build and flash the project using ESP-IDF tools.

Usage:
    python setup_cyd_lvgl.py
"""

import os
import json
import shutil
import subprocess

# Get current script directory, project root, components directory, and settings.json path
current_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.abspath(os.path.join(current_dir, '../..'))
components_dir = os.path.join(root_dir, 'components')
setting_json_path = os.path.abspath(os.path.join(current_dir, '../..', '.vscode', 'settings.json'))

# Load json data from settings.json
data = None
with open(setting_json_path, 'r') as file:
    data = json.load(file)

# Load ESP-IDF path from settings.json
idf_path = data.get('idf.espIdfPathWin')

# --------- Clone required repositories ---------
# Prepare a batch script to clone LVGL, TFT_eSPI, and XPT2046_Touchscreen into components/
cmd = f"""
@echo off
@REM This is generated file, do not edit it manually
cd {root_dir}/components
git clone --branch v8.4.0 --single-branch https://github.com/lvgl/lvgl.git
git clone --branch V2.5.43 --single-branch https://github.com/Bodmer/TFT_eSPI.git
git clone --branch v1.4 --single-branch https://github.com/PaulStoffregen/XPT2046_Touchscreen.git

"""

# Write and run the batch file to clone repositories
bat_file_name = "clone_repo.bat"
bat_file_path = os.path.join(current_dir, bat_file_name)
with open(bat_file_path, "w", encoding="utf-8") as f:
    f.write(cmd)
subprocess.run([bat_file_path], shell=True)

# --------- Copy CMakeLists.txt to XPT2046_Touchscreen ---------
cmake_dir = os.path.abspath(os.path.join(root_dir, 'script/cmake_for_any_lib'))
XPT2046_dir = os.path.abspath(os.path.join(components_dir, 'XPT2046_Touchscreen'))
shutil.copyfile(os.path.join(cmake_dir, "CMakeLists.txt"), os.path.join(XPT2046_dir, "CMakeLists.txt"))

# --------- Copy main.cpp and sdkconfig.defaults to project ---------
main_dir = os.path.abspath(os.path.join(root_dir, 'main'))
shutil.copyfile(os.path.join(current_dir, "main.cpp"), os.path.join(main_dir, "main.cpp"))
shutil.copyfile(os.path.join(current_dir, "sdkconfig.defaults"), os.path.join(root_dir, "sdkconfig.defaults"))

# --------- Copy ui folder to components/ui ---------
shutil.copytree(os.path.join(current_dir, "ui"), os.path.join(components_dir, "ui"), dirs_exist_ok=True)

# --------- Generate batch file to build and flash the project ---------
cmd = f"""
@echo off
@REM This is generated file, do not edit it manually
call "{idf_path}\\export.bat"
idf.py fullclean
idf.py build
idf.py flash
"""

# Write the batch file for building and flashing
bat_file_name = "build_flash.bat"
bat_file_path = os.path.join(current_dir, bat_file_name)
with open(bat_file_path, "w", encoding="utf-8") as f:
    f.write(cmd)
subprocess.run([bat_file_path], shell=True)