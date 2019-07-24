#!/bin/bash

export G_TEST_IMAGE=srzzumix/googletest
export RUN_TEST_IMAGE=471pa2test

# Download image if it doesn't exist.
if [[ "$(docker images -q ${G_TEST_IMAGE} 2> /dev/null)" == "" ]]; then
  docker pull "${G_TEST_IMAGE}"
fi

# Copy source code here so it can be used.
cp -r ../src .
rm src/main.cpp

# Build Docker image and run Docker container
docker build -t "${RUN_TEST_IMAGE}" .
docker run "${RUN_TEST_IMAGE}"

# Remove source code once done
rm -rf src