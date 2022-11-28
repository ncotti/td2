import sys
import serial
import time
from queue import Queue
from widgets.cam_tab_widget import CamTabWidget
from widgets.image_displayer import ImageDisplayer
from widgets.motor_tab_widget import MotorTabWidget
from widgets.pie_chart import PieChart
from PyQt5.QtWidgets import QApplication,QWidget, QHBoxLayout, QTabWidget
import uart_tx
import uart_rx
from const import *


class MainWidget(QWidget):
    def __init__(self):
        super().__init__()
        self.rawImageQueue = Queue(0)
        self.txQueue = Queue(0)
        self.uart = False

        self.waitForSerial()

        self.uart_tx_th = uart_rx.UartRx(self.uart, self.rawImageQueue)
        self.uart_rx_th = uart_tx.UartTx(self.uart, self.txQueue)
        
        self.layout = QHBoxLayout()
        self.tabs = QTabWidget()
        self.tab_cam = CamTabWidget(self.rawImageQueue, self.txQueue)
        self.tab_motor = MotorTabWidget(self.txQueue)
        self.tabs.resize(300,300)

        self.image_displayer = ImageDisplayer(self.rawImageQueue)

        #TODO
        self.pie_chart = PieChart()

        self.tabs.addTab(self.tab_cam, "Camera")
        self.tabs.addTab(self.tab_motor, "Motor")

        self.layout.addWidget(self.tabs)
        self.layout.addWidget(self.image_displayer.imageLabel)
        self.layout.addWidget(self.pie_chart)
        
        self.setLayout(self.layout)
        self.setGeometry(300, 300, 300, 300)
        self.setWindowTitle("Cookie Conuter")

        self.uart_tx_th.start()
        self.uart_rx_th.start()

        self.show()


    def waitForSerial(self):
        while (not self.uart):
            try:
                self.uart = serial.Serial(PORT0, BAUD_RATE, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=5)
            except:
                try:
                    self.uart = serial.Serial(PORT1, BAUD_RATE, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=5)
                except:
                    pass
            finally:
                if (not self.uart):
                    print("Please, connect a serial device")
                    time.sleep(3)

                else:
                    print("Serial device found!")


        

if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = MainWidget()
    out = app.exec_()
    sys.exit(out)


