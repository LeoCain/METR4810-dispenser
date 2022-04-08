""" 
real-time plotting function used to display the state of
a specified pin.
"""
import pigpio as pp
from matplotlib import pyplot as plt
import numpy as np

# define pin to be tested:
test_pin = 24   # 24 for IR1, 25 for HAND
# Initialise pigpio object and read pin
pi = pp.pi()
pi.set_mode(test_pin, pp.INPUT)

# read from pin
pi.read(test_pin)

# plot input real time
x=-0.04
while True:
    x=x+0.04
    y = pi.read(test_pin)
    plt.scatter(x, y)
    plt.title("Real Time plot")
    plt.xlabel("x")
    plt.ylabel("pin input")
    plt.xlim(50)
    if pi.read(test_pin) == 1 :
        pi.set_pull_up_down(test_pin, pp.PUD_DOWN)
    else :
        pi.set_pull_up_down(test_pin, pp.PUD_UP)
    plt.pause(0.01)

# plt.show()


