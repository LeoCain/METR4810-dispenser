.DEFAULT_GOAL:=Allfiles

Allfiles: Functional Test_Suite 

Functional: Main IPDisp

Test_Suite: detect_tst IR rolltest doorservo SSDtest stepp slp_stepp LED ALL_SENSOR_debug

### Functional code ###
# Main functional code
Main: Run_Code/main_code_v2.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o RunCodeExec/main2 $^ -lpigpio -lrt
# IP display on startup code
IPDisp: Run_Code/IP_Display.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o IP_Display $^ -lpigpio -lrt

### Test Suite ###
# Hand detection code:
detect_tst: Test_Suite/detect_test.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o TestSuiteExec/detect_test $^ -lpigpio -lrt
# IR and IR LED code:
IR: Test_Suite/In_IRtest.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o TestSuiteExec/In_IRtest $^ -lpigpio -lrt
# multithreading test code:
# mthreading: Test_Suite/multithread_test.c
# 	gcc -Wall -pthread -o TestSuiteExec/mt_test $^ -lpigpio -lrt
# roller motor test code:
rolltest: Test_Suite/roll_test.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o TestSuiteExec/roll_test $^ -lpigpio -lrt
# door Servo test code:
doorservo: Test_Suite/Servo_test.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o TestSuiteExec/Servo_test $^ -lpigpio -lrt
# SSD test code:
SSDtest: Test_Suite/SSD_test.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o TestSuiteExec/SevSegtest $^ -lpigpio -lrt
# stepper test code:
stepp: Test_Suite/stepper_test.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o TestSuiteExec/stepp_test $^ -lpigpio -lrt
# stepper sleep code:
slp_stepp: Test_Suite/detach_stepp.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o TestSuiteExec/detach $^ -lpigpio -lrt
# LED test code:
LED: Test_Suite/LEDtest.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o TestSuiteExec/LEDtest $^ -lpigpio -lrt
# debugging of IR sensors
ALL_SENSOR_debug: Test_Suite/ALLSENS_debug.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o TestSuiteExec/ALLSENS_debug $^ -lpigpio -lrt 
