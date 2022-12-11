from PyQt5.QtCore import QThread
import serial
from queue import Queue
from const import *



class UartRx (QThread):
    """
    Thread for reading the images from the camera. The image is written
    to the queue.
    """
    def __init__ (self, uart:serial.Serial, queue:Queue):
        QThread.__init__(self)
        self.uart = uart
        self.queue = queue

    def run(self):
        """
        An image is read with the following format:
        SOF_HEADER
        <first_row>EOR_FOOTER
        <second_row>EOR_FOOTER
        EOF_FOOTER
        """
        rows = 0
        image = b""
        counter = 0

        while(True):
            buffer = self.uart.read_until(b"__\n")

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
                    #print (f"Ok: {counter}")
                    counter = counter + 1
                    self.queue.put(image)
                    
                image = b""
                rows = 0