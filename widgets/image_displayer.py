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
        self.frameLabel.setMaximumHeight(30)


        self.imageLabel = QLabel()
        grey = QPixmap(640, 480)
        grey.fill(QColor("darkGray"))
        self.imageLabel.setPixmap(grey)
        self.imageLabel.setMinimumWidth(640)
        self.imageLabel.setMinimumHeight(480)
        self.chart = chart

        self.addWidget(self.frameLabel)
        self.addWidget(self.imageLabel)

        #self.

        self.ImageProcessingTh = ImageProcessing(queue, self.imageLabel, self.frameLabel, self.chart, self)
        self.ImageProcessingTh.start()


class ImageProcessing(QThread):
    """
    This thread makes all the image processing neccesary.
    It receives raw bytes from the "rawImageQueue", and it sends
    a displayable image to the "displayImageQueue", raising a signal
    to "new_image_signal" when a new image is ready to be displayed.
    """
    def __init__ (self, rawImageQueue:Queue, imageLabel:QLabel, frameLabel:QLabel, chart:Chart, layout:QVBoxLayout):
        QThread.__init__(self)
        self.rawImageQueue = rawImageQueue
        self.imageLabel = imageLabel
        self.frameLabel = frameLabel
        self.model = torch.hub.load('ncotti/yolov5', 'custom', path=PT_PATH, force_reload=True)
        self.detections = []
        self.frame_counter = 0
        self.chart = chart
        self.layout = layout


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
                        
                if (rows):
                    # Por cada elemento detectado
                    for i in range(0, rows):
                        min_detection = None
                        min_distance = 10000

                        pos = ((dataFrame.at[i, "xmin"] + dataFrame.at[i, "xmax"])/2, (dataFrame.at[i, "ymin"] + dataFrame.at[i, "ymax"])/2)

                        # Por cada detección previamente registrada, hallar aquella con la mínima distancia
                        for detection in self.detections:
                            # Borrar la detección si la última vez que fue detectada fue hace 5 frames.
                            if (detection.get_last_frame() - self.frame_counter > 5):
                                self.detections.remove(detection)

                            elif (detection.get_distance(pos) < min_distance and detection.get_name() == dataFrame.at[i, "name"]):
                                min_distance = detection.get_distance(pos)
                                min_detection = detection

                        # Si todas las demás están lejos y está se encuentra
                        # al principio de la imagen, entonces es una nueva detección
                        if (pos[1] >= 80 and min_distance > 70 ):
                            self.detections.append( Detection(pos, dataFrame.at[i, "name"], self.frame_counter, self.chart))

                        # Se asocia con el dataframe más cercano actualmente
                        elif (min_detection):
                            min_detection.update_pos(pos, dataFrame.at[i, "name"], self.frame_counter)
                            
                            if (pos[1] <= 80 and pos[1] >= 40):
                                min_detection.update_middle_frame()

                            if (pos[1] <= 40 and detection.get_name() == dataFrame.at[i, "name"] and min_detection.get_middle_frame()):
                                min_detection.detected()
                                self.detections.remove(min_detection)
                        

                self.imageLabel.setPixmap(self.convert_cv_qt(results.ims[0]))


    def convert_cv_qt(self, raw_image:cv2.Mat):
        """Convert from an opencv image to QPixmap"""
        convert_to_Qt_format = QImage(raw_image, IMAGE_W, IMAGE_H, QImage.Format.Format_BGR888).scaledToWidth(self.imageLabel.width())
        return QPixmap.fromImage(convert_to_Qt_format)


class Detection():
    def __init__(self, pos:tuple, name:str, frame:int, chart:Chart):
        self.middle_frame = False
        self.chart = chart
        self.update_pos(pos, name, frame)

    def update_pos(self, pos:tuple, name:str, frame:int):
        self.pos = pos
        self.name = name
        self.last_frame = frame

    def update_middle_frame(self):
        self.middle_frame = True

    def get_middle_frame(self):
        return self.middle_frame

    def get_pos(self):
        return self.pos

    def get_distance(self, external_pos:tuple):
        return np.sqrt((self.pos[0] - external_pos[0])**2 + (self.pos[1] - external_pos[1])**2 )

    def get_last_frame(self):
        return self.last_frame

    def get_name(self):
        return self.name

    def detected(self):
        self.chart.update_bar(self.name)