import serial
from datetime import datetime as time
t = time.now()
print(t)
if __name__ == '__main__':
    ser = serial.Serial('COM6', 57600)
    ser.reset_input_buffer()
    while True:

        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(type(line))
            t1 = time.now()
            print(line,t1-t)
            t = t1