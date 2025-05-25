#!/bin/bash

echo Selecting option to build docker image.
echo "1. Using image with latest esp-idf version"
echo "2. Using image with specific esp-idf version"
echo
read -p "Enter your choice (1 or 2): " choice

# Delete Dockerfile and devcontainer.json if they exist in the current folder
rm -f ./Dockerfile ./devcontainer.json ./entrypoint.sh

if [ "$choice" == "1" ]; then
    cp ./template/devcontainer_original.json ./devcontainer.json
    curl -o $PWD/Dockerfile https://raw.githubusercontent.com/espressif/esp-idf/master/tools/docker/Dockerfile
    curl -o $PWD/entrypoint.sh https://raw.githubusercontent.com/espressif/esp-idf/master/tools/docker/entrypoint.sh
elif [ "$choice" == "2" ]; then
    cp ./template/devcontainer_customized.json ./devcontainer.json
    curl -o $PWD/Dockerfile https://raw.githubusercontent.com/espressif/esp-idf/master/tools/docker/Dockerfile
    curl -o $PWD/entrypoint.sh https://raw.githubusercontent.com/espressif/esp-idf/master/tools/docker/entrypoint.sh
else
    echo "Invalid choice."
fi

echo "Done."