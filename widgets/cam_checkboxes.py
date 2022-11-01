from PyQt5.QtWidgets import QWidget, QCheckBox, QGridLayout, QGroupBox, QBoxLayout
from const import *
from queue import Queue

class CamCheckBoxes(QWidget):
    def __init__(self, queue:Queue):
        super().__init__()
        self.queue = queue
        self.lay = QBoxLayout(0)
        self.group_lay = QGridLayout()
        self.group = QGroupBox()

        self.hMirror = SingleCheckBox("Horizontal Mirror", REG_HMIRROR_ON, REG_HMIRROR_OFF, self.queue)
        self.vFlip = SingleCheckBox("Vertical Flip", REG_VFLIP_ON, REG_VFLIP_OFF, self.queue)
        self.testPattern = SingleCheckBox("Test Pattern", REG_TEST_ON, REG_TEST_OFF, self.queue)
        self.colorBar = SingleCheckBox("Color Bar", REG_COLOR_BAR_ON, REG_COLOR_BAR_OFF, self.queue)
        self.blockAgc = SingleCheckBox("Block AGC/AEC", REG_FREEZE_AGC, REG_UNFREEZE_AGC, self.queue)

        self.group_lay.addWidget(self.hMirror, 0, 0)
        self.group_lay.addWidget(self.vFlip, 1, 0)
        self.group_lay.addWidget(self.testPattern, 2, 0)
        self.group_lay.addWidget(self.colorBar, 3, 0)
        self.group_lay.addWidget(self.blockAgc, 4, 0)

        self.group.setLayout(self.group_lay)
        self.lay.addWidget(self.group)

        self.setLayout(self.lay)
        self.reset()

    def reset(self):
        self.hMirror.reset()
        self.vFlip.reset()
        self.testPattern.reset()
        self.colorBar.reset()
        self.blockAgc.reset()




class SingleCheckBox(QCheckBox):
    def __init__(self, title:str, reg_on:str, reg_off:str, queue:Queue):
        super().__init__(title)
        self.pressed.connect(self.on_pressed)
        self.reg_on = reg_on
        self.reg_off = reg_off
        self.queue = queue

    def on_pressed(self):
        if not self.isChecked():
            self.queue.put(self.reg_on)
        else:
            self.queue.put(self.reg_off)

    def reset(self):
        self.setChecked(False)