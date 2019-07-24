# Download base image ubuntu 16.04.
FROM ubuntu:16.04

# whoami
LABEL maintainer="kyler.j.little@gmail.com"

# Update Software repository
RUN apt-get update

# Install gcc, g++, make, C libs, package dev tools & say 'Y' automatically
RUN apt-get -y install build-essential

# Establish workdir
ENV code_dir /usr/src/
WORKDIR ${code_dir}

# Copy current directory to code directory on Ubuntu image.
COPY . ${code_dir}

# Start bash and run program
CMD ["/bin/bash"]