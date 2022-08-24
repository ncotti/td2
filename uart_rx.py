from PyQt5.QtCore import QThread
from const import *

g_stop = 0

class UartRx (QThread):
    def __init__ (self, uart, queue):
        QThread.__init__(self)
        self.uart = uart
        self.queue = queue

    def run(self):
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



    




