from queue import Queue
import serial
from const import *
from PyQt5.QtCore import QThread

g_stop = 0

class UartTx (QThread):
    """
    It receives from a queue from the GUI the registers to be modified,
    and sends them through UART in the following fashion:
    0xFA    Address     Value               0xFE
    0xFA    0xFF        Camera_function     0xFE
    0xFA    0xFE        Motor_function      0xFE
    """
    def __init__ (self, uart:serial.Serial, queue:Queue):
        QThread.__init__(self)
        self.uart = uart
        self.queue = queue

    def run(self):
        global g_stop
        while (not g_stop):
            try:
                msg = self.queue.get(block=True, timeout=1000)
                buf = bytes((0xFA, msg[0], msg[1], 0xFE))
                self.uart.write(buf)
            except:
                pass
    
    
