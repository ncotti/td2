import cv2
import numpy as np
import serial
from const import *


"""
An image is read with the following format:
SOF_HEADER
<first_row>EOR_FOOTER
<second_row>EOR_FOOTER
EOF_FOOTER
"""
uart = serial.Serial(PORT0, BAUD_RATE, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=5)
rows = 0
image = b""
counter = 0

while(True):
    buffer = uart.read_until(b"__\n")

    end_row = len(buffer) - len(EOR_FOOTER)
    aux = buffer[end_row : len(buffer)]

    if aux == EOR_FOOTER:
        image = image + buffer[0:end_row]
        rows = rows + 1

    elif aux == SOF_HEADER:
        image = b""
        rows = 0

    elif aux == EOF_FOOTER:
        if (rows != IMAGE_H):
            print (f"Wrong amount of rows: {rows}")

        elif (len(image) != IMAGE_BYTES):
            print (f"Wrong Image size: {len(image)}")

        else:
            print (f"Ok: {counter}")
            counter = counter + 1
            #self.queue.put(image)
            
            
            image = np.frombuffer(image, np.uint8).reshape(IMAGE_H, IMAGE_W, BYTES_PER_PIXEL)
            image = cv2.cvtColor(image, cv2.COLOR_YUV2RGB_Y422)
            #image = cv2.cvtColor(image, cv2.COLOR_BGR5652BGR)
            cv2.imshow("test", image)
            cv2.waitKey(1)
            
        image = b""
        rows = 0