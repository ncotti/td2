from PyQt5.QtWidgets import QWidget, QLabel
from PyQt5.QtChart import QPieSeries, QPieSlice
from PyQt5.QtCore import Qt, QThread, pyqtSignal
from const import *
from queue import Queue, Empty
from PyQt5.QtGui import QImage, QPixmap, QColor
import cv2
import numpy as np

class PieChart(QWidget):
    def __init__(self, queue:Queue):
        super().__init__()
        self.chart = QPieSeries()
        self.chart