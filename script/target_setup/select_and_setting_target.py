"""
This script automates switching and building for ESP32/ESP32C3 targets in an ESP-IDF project.

Steps performed:
- Validates the target argument (must be 'esp32' or 'esp32c3').
- Loads the ESP-IDF path from .vscode/settings.json.
- Updates the idf.openOcdConfigs in settings.json based on the selected target
- Copies the target's CMakeLists.txt and sdkconfig.defaults to the project root.
- Deletes the build directory for a clean build.
- Generates a batch file to set up the environment and build/flash the project.
- Executes the generated batch file.
"""

import os
import json
import subprocess
import sys
import shutil
import platform

target = None
# Validate parameter
valid_targets = {"esp32", "esp32c3"}
if len(sys.argv) > 1 and sys.argv[1] in valid_targets:
    target = sys.argv[1]
else:
    print("Usage: python select_target.py [esp32|esp32c3]")
    sys.exit(1)

# Get current script directory and settings.json path
current_dir = os.path.dirname(os.path.abspath(__file__))
setting_json_path = os.path.abspath(os.path.join(current_dir, '../..', '.vscode', 'settings.json'))

# Get json data from settings.json
data = None
with open(setting_json_path, 'r') as file:
    data = json.load(file)

# Load ESP-IDF path from settings.json
idf_path = os.environ.get('IDF_PATH')
if not idf_path:
    print("Error: IDF_PATH environment variable is not set")
    sys.exit(1)

# Update idf.openOcdConfigs according to selected target
if target == "esp32c3":
    data["idf.openOcdConfigs"] = ["board/esp32c3-builtin.cfg"]
elif target == "esp32":
    data["idf.openOcdConfigs"] = ["board/esp32-wrover-kit-3.3v.cfg"]

# Write the updated settings.json back
with open(setting_json_path, 'w', encoding='utf-8') as file:
    json.dump(data, file, indent=2)

# Copy CMakeLists.txt and sdkconfig.defaults from target folder to project root
root_dir = os.path.abspath(os.path.join(current_dir, '../..'))
target_artifact_dir = os.path.join(current_dir, target)
shutil.copyfile(os.path.join(target_artifact_dir, "CMakeLists.txt"), os.path.join(root_dir, "CMakeLists.txt"))
shutil.copyfile(os.path.join(target_artifact_dir, "sdkconfig.defaults"), os.path.join(root_dir, "sdkconfig.defaults"))

# Delete the build folder in the root directory if it exists
# Reason of deletion: idf.py set-target requires a clean build
build_dir = os.path.join(root_dir, "build")
if os.path.exists(build_dir):
    shutil.rmtree(build_dir)


cmd = ""
if platform.system() == "Windows":
    cmd_extension = ".bat"
    cmd = f"""
@echo off
@REM This is generated file, do not edit it manually
call "{idf_path}\\export.bat"
"""
else:
    cmd_extension = ".sh"
    cmd = f"""#!/bin/bash
# This is generated file, do not edit it manually
source "{idf_path}/export.sh"
"""

# Create the batch script to set up environment and build/flash the project
# Why there is idf.py fullclean step?
# Because it ensures that all enviroments setting from previous build are removed then avoid build enviroments conflicts
cmd = cmd + f"""
idf.py set-target {target}
idf.py fullclean
idf.py build
idf.py flash
"""

# Prepare command file path for the selected target
cmd_file_name = target + cmd_extension
cmd_file_path = os.path.join(target_artifact_dir, cmd_file_name)

# Write the batch file
with open(cmd_file_path, "w", encoding="utf-8") as f:
    f.write(cmd)

# Run the batch file
os.chmod(cmd_file_path, 0o755)
subprocess.run([cmd_file_path], shell=True)