from PyQt5.QtCore import QThread
import serial
from queue import Queue
from const import *

g_stop = 0

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
        global g_stop
        rows = 0
        image = b""
        buffer = b""

        while(not g_stop):
            buffer = buffer + self.uart.read(1) 
        
            if SOF_HEADER in buffer:
                image = b""
                buffer = b""
                rows = 0
                
            elif EOR_FOOTER in buffer:
                image = image + buffer.removesuffix(EOR_FOOTER)
                buffer = b""
                rows = rows + 1

            elif EOF_FOOTER in buffer:
                if (rows != IMAGE_H):
                    print (f"Wrong amount of rows: {rows}")

                elif (len(image) != IMAGE_BYTES):
                    print (f"Wrong Image size: {len(image)}")

                else:
                    self.queue.put(image[0:IMAGE_BYTES])

                image = b""
                buffer = b""
                rows = 0



    




