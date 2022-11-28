import serial
import time
import numpy as np
import matplotlib.pyplot as plt

ser = serial.Serial('COM3', 115200, bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=None)

plt.axis([0, 1000, -20, 120])
xdata = []
ydata = []
piddata = []
setdata = []

x = [0.0,0.0]
y = [0.0,0.0]
i = 0

if ser.is_open:
    ser.write(str("0.0;").encode("utf-8"))
    while (True):
        try:
            if (ser.inWaiting()):
                x[1] = x[0]
                [setpoint, x[0]] = [float(v) for v in (ser.readline().decode("utf-8").split(";"))]

                y[1] = y[0]
                y[0] = 0.00413 * x[0] + 0.00413 * x[1] + 0.992 * y[1]

                ser.write(("%s;" % (str(y[0]))).encode("utf-8"))

                ydata.append(y[0])
                xdata.append(i)
                setdata.append(setpoint)
                piddata.append(x[0])

                i += 1
                print(str(y[0]) + str("   ") + str(x[0]))
                if (i % 10 == 0):
                    plt.clf()
                    plt.axis([0, i, -20, 120])
                    
                    # Plotting velocity, setpoint, adn PID output
                    plt.plot(xdata, ydata, label="Profil kecepatan")
                    plt.plot(xdata, setdata, label="Setpoint")
                    plt.plot(xdata, piddata, label="Output PID")
                    plt.legend()
                    plt.draw()
                    plt.pause(0.03)

        except Exception as e:
            plt.clf()
            plt.axis([0, i, -20, 120])
            plt.plot(xdata, ydata, label="Profil kecepatan")
            plt.plot(xdata, setdata, label="Setpoint")
            plt.plot(xdata, piddata, label="Output PID")
            plt.legend()
            plt.savefig("./hasil.jpg") # Saving picture
            print(e)
            break
