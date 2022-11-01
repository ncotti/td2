from PyQt5.QtWidgets import QWidget, QComboBox, QHBoxLayout, QVBoxLayout, QLabel
from const import *
from queue import Queue
from PyQt5.QtCore import Qt

class AWBWidget(QWidget):
    def __init__(self, queue:Queue):
        super().__init__()
        self.layout = QHBoxLayout()
        self.queue = queue

        self.awb_r = AWBComboBox("r", self.queue)
        self.awb_b = AWBComboBox("b", self.queue)
        self.awb_gr = AWBComboBox("gr", self.queue)
        self.awb_gb = AWBComboBox("gb", self.queue)

        self.layout.addItem(self.awb_r.layout)
        self.layout.addItem(self.awb_b.layout)
        self.layout.addItem(self.awb_gb.layout)
        self.layout.addItem(self.awb_gr.layout)

        self.reset()

        self.setLayout(self.layout)

    def reset(self):
        self.awb_r.combo.setCurrentIndex(0)
        self.awb_b.combo.setCurrentIndex(0)
        self.awb_gb.combo.setCurrentIndex(0)
        self.awb_gr.combo.setCurrentIndex(0)


class AWBComboBox(QWidget):
    def __init__(self, color:str, queue:Queue):
        super().__init__()
        
        self.layout = QVBoxLayout()
        self.queue = queue
        self.combo = QComboBox()
        self.combo.addItems(["x1", "x1.25", "x1.5", "x1.75"])
        self.label = QLabel("xxx")
        self.label.setAlignment(Qt.AlignmentFlag.AlignBottom | Qt.AlignmentFlag.AlignHCenter)

        color = color.lower()
        if (color == "r"):
            self.reg1 = REG_AWB_R1
            self.reg125 = REG_AWB_R125
            self.reg15 = REG_AWB_R15
            self.reg175 = REG_AWB_R175
            self.label.setText("Red")
        
        elif (color == "b"):
            self.reg1 = REG_AWB_B1
            self.reg125 = REG_AWB_B125
            self.reg15 = REG_AWB_B15
            self.reg175 = REG_AWB_B175
            self.label.setText("Blue")

        elif (color == "gr"):
            self.reg1 = REG_AWB_GR1
            self.reg125 = REG_AWB_GR125
            self.reg15 = REG_AWB_GR15
            self.reg175 = REG_AWB_GR175
            self.label.setText("Gr")

        elif (color == "gb"):
            self.reg1 = REG_AWB_GB1
            self.reg125 = REG_AWB_GB125
            self.reg15 = REG_AWB_GB15
            self.reg175 = REG_AWB_GB175
            self.label.setText("Gb")
            
        self.layout.addWidget(self.label)
        self.layout.addWidget(self.combo)
        self.combo.activated.connect(self.awb_cb)

    def awb_cb(self, index):
        selection = self.combo.currentText()

        if (selection == "x1"):
            self.queue.put(self.reg1)
        
        elif (selection == "x1.25"):
            self.queue.put(self.reg125)

        elif (selection == "x1.5"):
            self.queue.put(self.reg15)

        else:
            self.queue.put(self.reg175)