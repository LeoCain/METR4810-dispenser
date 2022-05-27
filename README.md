# METR4810-dispenser
Github repository link:
https://github.com/LeoCain/METR4810-dispenser.git

## Description:
A collection of:
- C header and source files
- Executable files
- python files

Used for testing and implementation of the METR4810 Mask Dispenser Project. This project aims to reliably dispense 50 individual surgical masks in under 20 minutes, while ensuring masks have zero contamination. The intended platform for implementation of this software is a Raspberry Pi 4.

## Dependencies:
Pigpio: please refer to the following link for installation instruction related to the pigpio library:

https://abyz.me.uk/rpi/pigpio/download.html

matplotlib: please execute the following command in an Ubuntu/Debian - based environment to install:
```linux
sudo apt-get install python3-matplotlib
```

## Overview:
The TP2 directory contains all the executables relevant to the project. This includes test code for various components, and the main executable for the complete dispenser project.

It also contains two directories:

Test_Suite is a test suite, which contains test code for all componentry which was implemented on the design.

Run_Code contains source and header files which define the main function of the process, a library which the main process depends on, plus header files to declare pinouts and other useful parameters.

## Credits:
Benjamin Von-Snarski; 45287008

Xavier Spinella; 45328107

Harrison Brown; 45821907

Hokul Srikanthan; 45286113
