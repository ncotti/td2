from queue import Queue
from widgets.text_register import TextRegisterWidget
from widgets.awb import AWBWidget
from widgets.cam_sliders import CamSliders
from widgets.cam_checkboxes import CamCheckBoxes
from PyQt5.QtWidgets import QWidget, QPushButton, QGridLayout
from const import *


class CamTabWidget(QWidget):
    def __init__(self, rawImageQueue:Queue, txQueue:Queue):
        super().__init__()
        
        self.txQueue = txQueue
        self.rawImageQueue = rawImageQueue        

        self.layout = QGridLayout()

        self.input_register = TextRegisterWidget(self.txQueue)
        self.awb = AWBWidget(self.txQueue)
        self.sliders = CamSliders(self.txQueue)
        self.check_boxes = CamCheckBoxes(self.txQueue)
        
        self.reset = QPushButton("Reset")
        self.reset.clicked.connect(self.reset_cb)

        self.layout.addWidget(self.reset, 4, 0)
        self.layout.addWidget(self.awb, 2, 0)
        self.layout.addWidget(self.input_register, 3, 0)
        self.layout.addWidget(self.sliders, 1, 0)
        self.layout.addWidget(self.check_boxes, 0, 0)

        self.setLayout(self.layout)


    def reset_cb(self):
        self.txQueue.put(REG_RESET)
        self.awb.reset()
        self.sliders.reset()
        self.check_boxes.reset()