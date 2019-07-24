#!/bin/bash

# NOTE:
# The -it instructs Docker to allocate a pseudo-TTY connected to the 
# container’s stdin, creating an interactive bash shell in the container
DOCKER_IMAGE=kylerlittle/class-work:471_pa2

# Check if docker is installed and executable.
if [ -x "$(command -v docker)" ]; then
    docker pull $DOCKER_IMAGE; \
    docker run -it $DOCKER_IMAGE
else
    echo "Install Docker please."
fi