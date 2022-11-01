
from queue import Queue
from PyQt5.QtWidgets import QLabel, QWidget, QGridLayout, QSlider
from widgets.cam_checkboxes import SingleCheckBox
from PyQt5.QtCore import Qt

from const import *


class MotorTabWidget(QWidget):
    def __init__(self, txQueue:Queue):
        super().__init__()

        self.txQueue = txQueue
        self.lay = QGridLayout()

        self.speedLabel = QLabel("Speed [mseg/step]")

        self.speed_slider = QSlider(Qt.Horizontal)
        self.speed_slider.setMinimum(5)
        self.speed_slider.setMaximum(100)
        self.speed_slider.setTickPosition(QSlider.TicksAbove)
        self.speed_slider.sliderReleased.connect(self.speed_cb)
        self.speed_slider.setValue(40)

        self.lay.addWidget(self.speedLabel, 2, 0)
        self.lay.addWidget(self.speed_slider, 2, 1)

        self.on_button = SingleCheckBox("On/Off", REG_MOTOR_ON, REG_MOTOR_OFF, self.txQueue)

        self.lay.addWidget(self.on_button, 0, 0)

        self.dir_button = SingleCheckBox("Forward/Backwards", REG_MOTOR_DIR, REG_MOTOR_DIR, self.txQueue)

        self.lay.addWidget(self.dir_button)

        self.setLayout(self.lay)

    def speed_cb(self):
        self.queue.put((0xFE, self.speed_slider.value()))