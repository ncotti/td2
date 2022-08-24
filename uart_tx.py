from queue import Queue
from const import *
from PyQt5.QtCore import QThread

g_stop = 0

class UartTx (QThread):
    def __init__ (self, uart, queue:Queue):
        QThread.__init__(self)
        self.uart = uart
        self.queue = queue

    def run(self):
        global g_stop
        while (not g_stop):
            try:
                msg = self.queue.get(block=True, timeout=1000)
                buf = bytes(f"#{msg}*", "utf-8")
                self.uart.write(buf)
            except:
                pass
    
    
