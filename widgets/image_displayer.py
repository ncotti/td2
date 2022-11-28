from PyQt5.QtWidgets import QWidget, QLabel
from PyQt5.QtCore import Qt, QThread, pyqtSignal
from const import *
from queue import Queue, Empty
from PyQt5.QtGui import QImage, QPixmap, QColor
import cv2
import numpy as np

import torch

class ImageDisplayer(QWidget):
    def __init__(self, queue:Queue):
        super().__init__()
        self.imageLabel = QLabel()
        grey = QPixmap(self.width(), self.height())
        grey.fill(QColor("darkGray"))
        self.imageLabel.setPixmap(grey)

        self.ImageProcessingTh = ImageProcessing(queue, self.imageLabel)
        self.ImageProcessingTh.start()


class ImageProcessing(QThread):
    """
    This thread makes all the image processing neccesary.
    It receives raw bytes from the "rawImageQueue", and it sends
    a displayable image to the "displayImageQueue", raising a signal
    to "new_image_signal" when a new image is ready to be displayed.
    """
    def __init__ (self, rawImageQueue:Queue, imageLabel:QLabel):
        QThread.__init__(self)
        self.rawImageQueue = rawImageQueue
        self.imageLabel = imageLabel
        self.model = torch.hub.load('ultralytics/yolov5', 'custom', path=PT_PATH)


    def run(self):
        while(True):        
            try:
                image = self.rawImageQueue.get(block=True, timeout=3)
            except Empty:
                image = 0

            if (image):
                image = np.frombuffer(image, np.uint8).reshape(IMAGE_H, IMAGE_W, BYTES_PER_PIXEL)
                image = cv2.cvtColor(image, cv2.COLOR_YUV2RGB_Y422)
                results = self.model(image)
                results.render()

                #self.imageLabel.setPixmap(results.ims[0])
                #image = self.convert_cv_qt(results.ims[0])

                self.imageLabel.setPixmap(self.convert_cv_qt(results.ims[0]))

                #self.imageLabel.setPixmap(self.convert_cv_qt(image))


    def convert_cv_qt(self, raw_image:cv2.Mat):
        """Convert from an opencv image to QPixmap"""
        convert_to_Qt_format = QImage(raw_image, IMAGE_W, IMAGE_H, QImage.Format.Format_BGR888).scaledToWidth(self.imageLabel.width())
        return QPixmap.fromImage(convert_to_Qt_format)