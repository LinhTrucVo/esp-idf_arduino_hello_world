#!/bin/bash
# This is generated file, do not edit it manually
source "/opt/esp/idf/export.sh"

idf.py set-target esp32c3
idf.py fullclean
idf.py build
idf.py flash
