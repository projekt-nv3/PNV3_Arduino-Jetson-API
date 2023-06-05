import serial
import time
import sys
from IPython.display import display, HTML

# latitude = 18.5204303 # Example latitude
# longitude = 73.8567437  # Example longitude

# ["b'-11111", '3216', '-864', '17056', '4240', '368', '235', '290.48', '233', "'"]
arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
# data = "b'18.5204303,73.8567437,234,-234,15221,5433,564,234,234'"
while True:
    try:
        data = arduino.readline()
        if data:
            data = [str(i) for i in str(data).split('|') if not i=="'"]
            # print(data)
            print("Location: ",data[0][2:])
            if data[0][2:] != "-111111":
                location = data[0][2:]
                l = location.split(',')
                latitude = l[0]
                longitude = l[1]

                latitude = 18.5204303 # Example latitude
                longitude = 73.8567437
                button_html = f'<button onclick="window.open(\'https://www.google.com/maps/search/?api=1&query={latitude},{longitude}\', \'_blank\')">Go to Location</button>'
                display(HTML(button_html))

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
