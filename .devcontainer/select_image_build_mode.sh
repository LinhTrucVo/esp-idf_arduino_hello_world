#!/bin/bash

echo "1. Using original devcontainer file"
echo "2. Using customized devcontainer file"
read -p "Enter your choice (1 or 2): " choice

# Delete Dockerfile and devcontainer.json if they exist in the current folder
rm -f ./Dockerfile ./devcontainer.json ./entrypoint.sh

if [ "$choice" == "1" ]; then
    cp ./template/Dockerfile_original ./Dockerfile
    cp ./template/devcontainer_original.json ./devcontainer.json
    echo "Original devcontainer files copied."
elif [ "$choice" == "2" ]; then
    cp ./template/devcontainer_customized.json ./devcontainer.json
    curl -o $PWD/Dockerfile https://raw.githubusercontent.com/espressif/esp-idf/master/tools/docker/Dockerfile
    curl -o $PWD/entrypoint.sh https://raw.githubusercontent.com/espressif/esp-idf/master/tools/docker/entrypoint.sh
    echo "Customized devcontainer file copied."
else
    echo "Invalid choice."
fi