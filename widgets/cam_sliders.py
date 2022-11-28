from PyQt5.QtWidgets import QWidget, QGridLayout, QLabel, QSlider
from PyQt5.QtCore import Qt
from const import *
from queue import Queue
import numpy as np

class CamSliders(QWidget):
    def __init__(self, queue:Queue):
        super().__init__()

        self.layout = QGridLayout()
        self.queue = queue

        self.layout.addWidget(QLabel("Brightness"), 0,0)
        self.layout.addWidget(QLabel("Contrast"), 1,0)
        
        self.brightness_slider = QSlider(Qt.Horizontal)
        self.brightness_slider.setMinimum(0)
        self.brightness_slider.setMaximum(255)
        self.brightness_slider.setTickPosition(QSlider.TicksAbove)
        self.brightness_slider.sliderReleased.connect(self.brightness_cb)
        self.layout.addWidget(self.brightness_slider, 0, 1)

        self.contrast_slider = QSlider(Qt.Horizontal)
        self.contrast_slider.setMinimum(0)
        self.contrast_slider.setMaximum(255)
        self.contrast_slider.setTickPosition(QSlider.TicksAbove)
        self.contrast_slider.sliderReleased.connect(self.contrast_cb)
        self.layout.addWidget(self.contrast_slider, 1, 1)
        
        self.setLayout(self.layout)
        self.reset()

    def reset(self):
        self.brightness_slider.setValue(0x80)
        self.contrast_slider.setValue(0x40)

    def brightness_cb(self):
        value = self.brightness_slider.value()
        if (value <= 0x80):
            value = (~value + 1) & 0xFF

        else:
            value = value & (~0x80)
        self.queue.put((0x55, value))

    def contrast_cb(self):
        self.queue.put((0x56, self.contrast_slider.value()))

    