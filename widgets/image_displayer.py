from PyQt5.QtWidgets import QWidget, QLabel, QVBoxLayout
from PyQt5.QtCore import Qt, QThread, pyqtSignal
from const import *
from queue import Queue, Empty
from PyQt5.QtGui import QImage, QPixmap, QColor
from widgets.chart import Chart
import cv2
import numpy as np

import torch

class ImageDisplayer(QVBoxLayout):
    def __init__(self, queue:Queue, chart:Chart):
        super().__init__()

        self.frameLabel = QLabel("Frame: 0")
        self.frameLabel.setAlignment(Qt.AlignmentFlag.AlignHCenter | Qt.AlignmentFlag.AlignVCenter)
        self.imageLabel = QLabel()
        grey = QPixmap(640, 480)
        grey.fill(QColor("darkGray"))
        self.imageLabel.setPixmap(grey)
        self.chart = chart

        self.addWidget(self.frameLabel)
        self.addWidget(self.imageLabel)

        self.ImageProcessingTh = ImageProcessing(queue, self.imageLabel, self.frameLabel, self.chart)
        self.ImageProcessingTh.start()


class ImageProcessing(QThread):
    """
    This thread makes all the image processing neccesary.
    It receives raw bytes from the "rawImageQueue", and it sends
    a displayable image to the "displayImageQueue", raising a signal
    to "new_image_signal" when a new image is ready to be displayed.
    """
    def __init__ (self, rawImageQueue:Queue, imageLabel:QLabel, frameLabel:QLabel, chart:Chart):
        QThread.__init__(self)
        self.rawImageQueue = rawImageQueue
        self.imageLabel = imageLabel
        self.frameLabel = frameLabel
        self.model = torch.hub.load('ncotti/yolov5', 'custom', path=PT_PATH, force_reload=True)
        self.detections = []
        self.frame_counter = 0
        self.chart = chart


    def run(self):
        while(True):        
            try:
                image = self.rawImageQueue.get(block=True, timeout=3)
            except Empty:
                image = 0

            if (image):
                self.frame_counter = self.frame_counter + 1
                self.frameLabel.setText(f"Frame: {self.frame_counter}")
                
                image = np.frombuffer(image, np.uint8).reshape(IMAGE_H, IMAGE_W, BYTES_PER_PIXEL)
                image = cv2.cvtColor(image, FORMAT)
                results = self.model(image, linewidth=1, fontsize=12)
                results.render()
                dataFrame = results.pandas().xyxy[0]
                rows = dataFrame.shape[0]

                for i in range(0, rows):
                    min_pos_diff = 1000
                    min_detection = None
                    for detection in self.detections:
                        if (detection.get_pos_diff(dataFrame, i) < min_pos_diff):
                            min_pos_diff = detection.get_pos_diff(dataFrame, i)
                            min_detection = detection

                    if (dataFrame.at[i, "ymax"] > 110 and min_pos_diff > 10 and dataFrame.at[i, "confidence"] > 0.5):
                        self.detections.append( Detection(dataFrame, i, self.chart))

                    elif (min_detection):
                        min_detection.update_pos(dataFrame, i)

                        if (dataFrame.at[i, "ymin"] < 10 and min_detection.get_frames() > 3):
                            min_detection.detected()
                            self.detections.remove(min_detection)
                        

                self.imageLabel.setPixmap(self.convert_cv_qt(results.ims[0]))


    def convert_cv_qt(self, raw_image:cv2.Mat):
        """Convert from an opencv image to QPixmap"""
        convert_to_Qt_format = QImage(raw_image, IMAGE_W, IMAGE_H, QImage.Format.Format_BGR888).scaledToWidth(self.imageLabel.width())
        return QPixmap.fromImage(convert_to_Qt_format)

class Detection():
    def __init__(self, dataFrame, i, chart:Chart):
        self.frames = 0
        self.confidence = 0
        self.chart = chart
        self.update_pos(dataFrame, i)

    def update_pos(self, dataFrame, i):
        self.ymin = dataFrame.at[i, "ymin"]
        self.ymax = dataFrame.at[i, "ymax"]
        self.xmin = dataFrame.at[i, "xmin"]
        self.xmax = dataFrame.at[i, "xmax"]
        confidence = dataFrame.at[i, "confidence"]
        self.confidence = confidence if confidence >= self.confidence else self.confidence
        self.name = dataFrame.at[i, "name"] if confidence >= self.confidence else self.name
        self.frames = self.frames + 1

    def get_pos_diff(self, dataFrame, i):
        return ((abs(self.ymin - dataFrame.at[i, "ymin"]) + 
            abs(self.ymax - dataFrame.at[i, "ymax"]) + 
            abs(self.xmin - dataFrame.at[i, "xmin"]) + 
            abs(self.xmax - dataFrame.at[i, "xmax"])) / 4)

    def get_frames(self):
        return self.frames

    def get_name(self):
        return self.name

    def detected(self):
        self.chart.update_bar(self.name)