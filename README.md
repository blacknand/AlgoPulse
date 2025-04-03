This guide explains how to build and run the AlgoPulse project, which includes two executables:
- **hftbot:** The main trading bot that receives live market data and processes it.
- **publisher:** A tool that simulates a live market data feed using ZeroMQ.

## Prerequisites

Before building the project, ensure you have the following installed:
- **CMake** (version 3.10 or later)
- A modern C++ compiler with C++17 support (e.g., g++ or clang++)
- **Boost** (version 1.65 or later, with the `system` component)
- **ZeroMQ** (libzmq3-dev) and **PkgConfig**
- **Threads** (standard in most systems)

On Ubuntu/Debian, you can install the required packages with:
```bash
sudo apt-get update
sudo apt-get install cmake build-essential libboost-all-dev libzmq3-dev pkg-config
```
On macOS:
```bash
brew update
brew install cmake boost zeromq pkg-config
```
On Windows:
```
Use WSL instead
```

## Build
```bash
mkdir build && cd build
cmake .. 
cmake --build .
```

## Running executables
- Publisher
```bash
./publisher
```
- Start the HFT bot
```bash
./hftbot
