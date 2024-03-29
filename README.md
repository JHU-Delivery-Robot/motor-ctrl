# Motor control and Encoder reading

Motor and Encoder firmware for JHU Deliverbot project

# Installing & Building

Only Linux is supported for building.

You will need to install the GCC ARM compilers, CMake, and Git. The GCC ARM compilers can be install via
```
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
```

Then clone this repository with submodules:
```
git clone --recurse-submodules https://github.com/JHU-Delivery-Robot/motor-ctrl.git
```
and from inside of `./motor-ctrl/pico-sdk` run `git submodule update --init`.

Next, go back into the top level of the git repository (`./`) and configure CMake with
```
mkdir build
cd build
cmake ..
```
and build with
```
make all
```