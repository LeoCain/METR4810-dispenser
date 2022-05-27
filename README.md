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

## Operation Manual:
PLEASE NOTE: This software is written for a specific robot with specific mechatronic design aspects. For details on the design, and how it should be used (i.e. how to load/reload masks), please see the robot manual.

# SSH connection:
The IP address of the PI will be displayed on startup:
```linux
ssh xspinella@ip.displayed.on.rpi
password: xspinella
```

# Configuration:
```linux
cd METR4810-dispenser/TP2  # Navigate to the TP2 directory:
make  # Ensure all the executables are compiled up-to-date:
sudo ./RunCodeExec/main2  # Begin the program:
```

At this stage, you will be asked to supply input with the following options:

 *0 (detach -> home)*: This is the option you should choose in most cases. It will detach the stepper so that it will freespin,   then the system will wait for you to press enter. Line up the installation marks, the press enter and let go of the             magazine. The magazine will then home, aligning the system.

 *1 (Home)*: This will only home the system.

 *2 (Detach)*: This will detach the stepper until the user presses enter

 *Any other entry*: Do nothing -> move to next stage

Following this, the system will ask for the stock level. Enter the number of masks in stock, press enter
Ensure that the magazine is loaded with masks, and install it.

# Customer Usage:
Customers should wave their hand 30 - 40 mm away from the mask request position (indicated on the front panel). The mask will take 5 - 10 seconds to dispense. Once the mask has stopped, and protrudes from the bottom of the dispenser, the customer should pull the mask straight down. After this, the door will close, and the dispenser will wait for the next request. 

The number displayed represents the number of masks remaining in stock. If the display shows an Err, and the Err does not clear in 10 - 20 seconds, the technician should be called.

# Troubleshooting:
If the seven segment display on the front displays an error, refer to the lid, or below for a description:

*Err0*: Mask is jammed between magazine and rollers.

*Err1*: Mask is jammed at the rollers.

*Err2*: Mask is jammed between rollers and door, or mask has not been taken:

*Err3*: The sensory input does not correspond to a valid state. This is usually caused by a malfunctioning sensor.

Other important information:

*The green LED*: This should illuminate when a mask is requested. If it does not, this means that the IR LEDs are not illuminated, and the system will not function correctly.

# Restocking:
If the seven segment display shows 0, there are no masks left in stock. The remote station will require you to signify whether you would like to re-stock, or quit the process.

If you choose to restock:

1. Enter the number of masks in the replacement magazine.
2. Swap the spent magazine with the replacement magazine, lining up the installation marks.
3. Press enter.
4. Wait for the stepper to home.
5. When the remote station displays "Waiting for mask request..." normal function has resumed.

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
