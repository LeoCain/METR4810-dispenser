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

## Overview of File Structure:
METR4810-dispenser: Contains Draw.io logic flow diagrams, and the TP2 project directory.
 

The TP2 directory contains a makefile, which compiles all of the test code, libraries, and main function software. It also contains the IP_Display executable, which is the only executable in this level as it is run on startup. There are also 5 directories:


Headers: Contains header files which define the raspberry pi pinout, and other important variables. It also contains a header file which declares all functions that are defined in the custom library source code.


Run_Code: Contains source files which define the main function of the process, and a library which the main process depends on. Also contains an old version of the main code.

RunCodeExec: Contains all the executables relevant to executing the main functionality of the dispenser.


Test_Suite: Contains source code for all individual components.


TestSuiteExec: Contains executables for the entire test suite.

## Credits:
Benjamin Von-Snarski; 45287008

Xavier Spinella; 45328107

Harrison Brown; 45821907

Hokul Srikanthan; 45286113
