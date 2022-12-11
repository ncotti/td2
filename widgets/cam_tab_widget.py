from queue import Queue
from widgets.text_register import TextRegisterWidget
from widgets.awb import AWBWidget
from widgets.cam_sliders import CamSliders
from widgets.cam_checkboxes import CamCheckBoxes
from widgets.chart import Chart
from PyQt5.QtWidgets import QWidget, QPushButton, QGridLayout, QHBoxLayout
from const import *


class CamTabWidget(QWidget):
    def __init__(self, txQueue:Queue, chart:Chart):
        super().__init__()
        
        self.txQueue = txQueue   
        self.chart = chart    

        self.layout = QGridLayout()

        self.input_register = TextRegisterWidget(self.txQueue)
        self.awb = AWBWidget(self.txQueue)
        self.sliders = CamSliders(self.txQueue)
        self.check_boxes = CamCheckBoxes(self.txQueue)
        
        self.reset_cam = QPushButton("Reset Config")
        self.reset_cam.clicked.connect(self.reset_cam_cb)

        self.reset_chart = QPushButton("Reset Graph")
        self.reset_chart.clicked.connect(self.reset_graph_cb)

        self.button_layout = QHBoxLayout()
        self.button_layout.addWidget(self.reset_cam)
        self.button_layout.addWidget(self.reset_chart)

        self.layout.addWidget(self.check_boxes, 0, 0)
        self.layout.addWidget(self.sliders, 1, 0)
        self.layout.addWidget(self.awb, 2, 0)
        self.layout.addWidget(self.input_register, 3, 0)
        self.layout.addLayout(self.button_layout, 4, 0)
        
        self.setLayout(self.layout)


    def reset_cam_cb(self):
        self.txQueue.put(REG_RESET)
        self.awb.reset()
        self.sliders.reset()
        self.check_boxes.reset()

    def reset_graph_cb(self):
        self.chart.reset()