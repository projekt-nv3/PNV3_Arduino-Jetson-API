import serial
import time
import sys
# ["b'-11111", '3216', '-864', '17056', '4240', '368', '235', '290.48', '233', "'"]
arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

while True:
    try:
        data = arduino.readline()
        if data:
            data = [str(i) for i in str(data).split('|') if not i=="'"]
            # print(data)
            print("Location: ",data[0][2:])
            print("AX: ",data[1])
            print("AY: ",data[2])
            print("AZ: ",data[3])
            print("GX: ",data[4])
            print("GY: ",data[5])
            print("GZ: ",data[6])
            print("Heading Angle: ",data[7])
            print("Distance bet object & vehicle: ",data[8])
            print("Light/Dark: ",data[9])

    except:
        arduino.close
        sys.exit()
