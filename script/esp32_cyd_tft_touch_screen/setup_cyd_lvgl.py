import os
import json
import shutil
import subprocess

# Get current script directory and settings.json and root path
current_dir = os.path.dirname(os.path.abspath(__file__))
root_dir = os.path.abspath(os.path.join(current_dir, '../..'))
components_dir = os.path.join(root_dir, 'components')
setting_json_path = os.path.abspath(os.path.join(current_dir, '../..', '.vscode', 'settings.json'))

# Get json data from settings.json
data = None
with open(setting_json_path, 'r') as file:
    data = json.load(file)

# Load ESP-IDF path from settings.json
idf_path = data.get('idf.espIdfPathWin')


cmd = f"""
@echo off
@REM This is generated file, do not edit it manually
cd {root_dir}/components
git clone --branch v8.4.0 --single-branch https://github.com/lvgl/lvgl.git
git clone --branch V2.5.43 --single-branch https://github.com/Bodmer/TFT_eSPI.git
git clone --branch v1.4 --single-branch https://github.com/PaulStoffregen/XPT2046_Touchscreen.git

"""

# Prepare batch file path for the selected target
bat_file_name = "clone_repo" + ".bat"
bat_file_path = os.path.join(current_dir, bat_file_name)

# Write the batch file
with open(bat_file_path, "w", encoding="utf-8") as f:
    f.write(cmd)

# Run the batch file
subprocess.run([bat_file_path], shell=True)

cmake_dir = os.path.abspath(os.path.join(root_dir, 'script/cmake_for_any_lib'))
XPT2046_dir = os.path.abspath(os.path.join(components_dir, 'XPT2046_Touchscreen'))
shutil.copyfile(os.path.join(cmake_dir, "CMakeLists.txt"), os.path.join(XPT2046_dir, "CMakeLists.txt"))

main_dir = os.path.abspath(os.path.join(root_dir, 'main'))
shutil.copyfile(os.path.join(current_dir, "main.cpp"), os.path.join(main_dir, "main.cpp"))

shutil.copyfile(os.path.join(current_dir, "sdkconfig.defaults"), os.path.join(root_dir, "sdkconfig.defaults"))
shutil.copytree(os.path.join(current_dir, "ui"), os.path.join(components_dir, "ui"), dirs_exist_ok=True)
