#!/bin/bash

echo Selecting option to build docker image.
echo "1. Using image with latest esp-idf version"
echo "2. Using image with specific esp-idf version"
echo

if [ -n "$1" ]; then
    choice="$1"
    echo "Choice provided as argument: $choice"
else
    read -p "Enter your choice (1 or 2): " choice
fi

# Delete Dockerfile and devcontainer.json if they exist in the current folder
rm -f ./Dockerfile ./devcontainer.json ./entrypoint.sh

# Function to download Docker files
download_docker_files() {
    curl -L -o .devcontainer/Dockerfile https://raw.githubusercontent.com/espressif/esp-idf/master/tools/docker/Dockerfile
    curl -L -o .devcontainer/entrypoint.sh https://raw.githubusercontent.com/espressif/esp-idf/master/tools/docker/entrypoint.sh
}

if [ "$choice" == "1" ]; then
    cp ./.devcontainer/template/devcontainer_original.json ./.devcontainer/devcontainer.json
    download_docker_files
elif [ "$choice" == "2" ]; then
    cp ./.devcontainer/template/devcontainer_customized.json ./.devcontainer/devcontainer.json
    download_docker_files
else
    echo "Invalid choice."
fi

echo "Done."