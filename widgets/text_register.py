from queue import Queue
from PyQt5.QtWidgets import QWidget, QPushButton, QLineEdit, QGridLayout, QLabel
from PyQt5.QtCore import Qt
class TextRegisterWidget(QWidget):
    def __init__(self, queue:Queue):
        super().__init__()
        self.queue = queue

        self.layout = QGridLayout()

        self.textReg = QLineEdit()
        self.textReg.setMaxLength(2)

        self.textValue = QLineEdit()
        self.textValue.setMaxLength(2)

        self.sendButton = QPushButton("Send")
        self.sendButton.clicked.connect(self.parseTextInput)

        self.addressLabel = QLabel("Address")
        self.addressLabel.setAlignment(Qt.AlignmentFlag.AlignBottom | Qt.AlignmentFlag.AlignHCenter)

        self.valueLabel = QLabel("Value")
        self.valueLabel.setAlignment(Qt.AlignmentFlag.AlignBottom | Qt.AlignmentFlag.AlignHCenter)

        self.layout.addWidget(self.addressLabel, 0, 0)
        self.layout.addWidget(self.valueLabel, 0, 1)
        self.layout.addWidget(self.textReg, 1, 0)
        self.layout.addWidget(self.textValue, 1, 1)
        self.layout.addWidget(self.sendButton, 1, 2)

        self.setLayout(self.layout)

    def parseTextInput(self):
        reg = self.textReg.text()
        value = self.textValue.text()

        if (len(reg) != 2 or len(value) != 2):
            self.textValue.setText("ER")

        try:
            reg = int(reg, 16)
            value = int(value, 16)
            self.queue.put((reg, value))
            self.textValue.setText("OK")
        except ValueError:
            self.textValue.setText("ER")