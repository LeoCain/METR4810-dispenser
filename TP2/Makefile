.DEFAULT_GOAL:=Allfiles

Allfiles: Functional Test_Suite 

Functional: Main IPDisp

Test_Suite: detect_tst IR

### Functional code ###
# Main functional code
Main: Run_Code/main_code_v2.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o RunCodeExec/main2 $^ -lpigpio -lrt
# IP display on startup code
IPDisp: Run_Code/IP_Display.c Run_Code/dispenser_lib.c
	gcc -Wall -pthread -o IP_Display $^ -lpigpio -lrt

### Test Suite ###
# Hand detection code:
detect_tst: Test_Suite/detect_test.c
	gcc -Wall -pthread -o TestSuiteExec/detect_test $^ -lpigpio -lrt

# IR and IR LED code:
IR: Test_Suite/detect_test.c
	gcc -Wall -pthread -o TestSuiteExec/In_IRtest $^ -lpigpio -lrt

# multithreading test code:
mthreading: Test_Suite/multithread_test.c
	gcc -Wall -pthread -o TestSuiteExec/mt_test $^ -lpigpio -lrt
