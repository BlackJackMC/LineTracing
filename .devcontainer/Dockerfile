FROM mcr.microsoft.com/devcontainers/python:1-3.11-bookworm

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    build-essential \
    clang \
    cmake \
    ninja-build \
    udev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN pip3 install --no-cache-dir -U platformio

RUN usermod -a -G dialout vscode && usermod -a -G plugdev vscode

WORKDIR /workspace