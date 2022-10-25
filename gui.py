import sys
import serial
from queue import Queue, Empty
import cv2
from PyQt5.QtWidgets import QApplication, QLabel, QWidget, QPushButton, QHBoxLayout, QVBoxLayout, QCheckBox, QSlider, QLineEdit
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt
import uart_tx
import image_process
import uart_rx
from const import *


class MyCheckBox(QCheckBox):
    def __init__(self, title:str, reg_on:str, reg_off:str, queue:Queue, parent:QWidget, layout:QVBoxLayout|QHBoxLayout):
        super().__init__(title, parent)
        super().pressed.connect(self.on_pressed)
        self.reg_on = reg_on
        self.reg_off = reg_off
        self.queue = queue
        layout.addWidget(self)

    def on_pressed(self):
        if not self.isChecked():
            if (self.reg_on == REG_RGB565):
                image_process.image_format = RGB565_FORMAT
            self.queue.put(self.reg_on)
        else:
            if (self.reg_off == REG_YUV):
                image_process.image_format = YUV_FORMAT
            self.queue.put(self.reg_off)


class Example(QWidget):
    def __init__(self):
        super().__init__()
        self.uart = serial.Serial("/dev/ttyACM0", 921600, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=10)
        

        self.bytes_queue = Queue(0)
        self.register_queue = Queue(0)
        self.image_queue = Queue(0)

        self.initUI()

        self.uart_tx_th = uart_rx.UartRx(self.uart, self.bytes_queue)
        self.uart_rx_th = uart_tx.UartTx(self.uart, self.register_queue)
        self.image_process_th = image_process.ImageProcessing(self.bytes_queue, self.image_queue, )
        self.uart_tx_th.start()
        self.uart_rx_th.start()
        self.image_process_th.start()
        self.image_process_th.new_image_signal.connect(self.display_image)
        self.show()


    def initUI(self):
        self.vbox = QVBoxLayout()
        self.hbox = QHBoxLayout()
        self.hbox_text = QHBoxLayout()

        self.okButton = QPushButton("OK")

        # TEXT
        self.sendButton = QPushButton("Send")
        self.sendButton.clicked.connect(self.parseTextInput)
        self.textReg = QLineEdit()
        self.textReg.setMaxLength(2)
        self.textValue = QLineEdit()
        self.textValue.setMaxLength(2)

        self.hbox_text.addWidget(self.textReg)
        self.hbox_text.addWidget(self.textValue)
        self.hbox_text.addWidget(self.sendButton)
        #END TEXT

        self.brightness_slider = QSlider(Qt.Horizontal)
        self.brightness_slider.setMinimum(0)
        self.brightness_slider.setMaximum(255)
        self.brightness_slider.setTickPosition(QSlider.TicksAbove)
        self.brightness_slider.valueChanged.connect(self.brightness_cb)

        self.vbox.addWidget(self.brightness_slider)

        self.hMirror = MyCheckBox("Horizontal Mirror", REG_HMIRROR_ON, REG_HMIRROR_OFF, self.register_queue, self, self.vbox)
        self.vFlip = MyCheckBox("Vertical Flip", REG_VFLIP_ON, REG_VFLIP_OFF, self.register_queue, self, self.vbox)
        self.testPattern = MyCheckBox("Test Pattern", REG_TEST_ON, REG_TEST_OFF, self.register_queue, self, self.vbox)
        self.colorBar = MyCheckBox("Color Bar", REG_COLOR_BAR_ON, REG_COLOR_BAR_OFF, self.register_queue, self, self.vbox)
        self.yuv = MyCheckBox("RGB565 / YUV", REG_RGB565, REG_YUV, self.register_queue, self, self.vbox)
        
        #self.XXX = MyCheckBox("XXX", 8, 8, self.register_queue, self, self.vbox)


        self.vbox.addItem(self.hbox_text)
        self.hbox.addLayout(self.vbox)

        self.setLayout(self.hbox)
        self.setGeometry(300, 300, 300, 300)
        self.setWindowTitle("title")

    def brightness_cb(self):
        print(self.brightness_slider.value())

    def parseTextInput(self):
        reg = self.textReg.text()
        value = self.textValue.text()

        if (len(reg) != 2 or len(value) != 2):
            self.textValue.setText("ER")

        try:
            reg = int(reg, 16)
            value = int(value, 16)
            self.register_queue.put((reg, value))
            self.textValue.setText("OK")
        except ValueError:
            self.textValue.setText("ER")
            

        


    def display_image(self):
        try:
            image = self.image_queue.get_nowait()
            cv2.namedWindow("title", cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)  # allow window resize (Linux)
            cv2.resizeWindow("title", 640, 480)
            cv2.imshow("title", image)
            cv2.waitKey(1)  # 1 millisecond

        except Empty:
            print("Error. Empty Queue")

    def destructor(self):
        image_process.g_stop = 1
        uart_rx.g_stop = 1
        uart_tx.g_stop = 1
        self.uart_tx_th.wait(1000)
        self.uart_rx_th.wait(100)
        self.image_process_th.wait(100)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = Example()
    out = app.exec_()
    w.destructor()
    sys.exit(out)


