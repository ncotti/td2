from queue import Empty, Queue
import cv2
import numpy as np
from const import *
from PyQt5.QtCore import QThread
import time
import torch
from PyQt5.QtCore import pyqtSignal

g_stop = 0
image_format = YUV_FORMAT

def rgb565_to_rgb888(image:bytes) -> bytes:
    rgbimage = bytearray()
    i = 0

    while(i < len(image)):
        r = (image[i+1] & 0xF8)
        g = (((image[i+1] & 0x07) << 5) | (image[i] >> 3)) & 0xFC
        b = (image[i] << 3) & 0xF8

        rgbimage.append(b)
        rgbimage.append(g)
        rgbimage.append(r)
        
        if (not i):
            print (image[i])
            print (image[i+1])
            print(r)
            print(g)
            print(b)
            print(rgbimage)
        

        i = i + 2

    return rgbimage


# Model
#model = torch.hub.load('ultralytics/yolov5', 'yolov5s')


class ImageProcessing(QThread):
    new_image_signal = pyqtSignal()

    def __init__ (self, bytes_queue:Queue, image_queue:Queue):
        QThread.__init__(self)
        self.bytes_queue = bytes_queue
        self.image_queue = image_queue

    def run(self):
        global g_stop, image_format
        while(not g_stop):

            # img1 = cv2.imread('zidane.jpg')  # PIL image
            # img2 = cv2.imread('bus.jpg')[:, :, ::-1]  # OpenCV image (BGR to RGB)
        
            try:
                image = self.bytes_queue.get(block=True, timeout=1000)
            except Empty:
                image = 0

        # with open("image.bayer", "rb") as file:
        #     image = file.read()
            if (image):
                image = np.frombuffer(image, np.uint8).reshape(IMAGE_H, IMAGE_W, BYTES_PER_PIXEL)
                
                if (image_format == YUV_FORMAT):
                    image = cv2.cvtColor(image, cv2.COLOR_YUV2BGR_Y422)

                else:
                    image = cv2.cvtColor(image, cv2.COLOR_BGR5652BGR)


                # This will show all pixels as blue, which is not what I expect.
                #results = model(image, size=153)  # includes NMS
                #results.show()

                #self.
                self.image_queue.put(image)
                self.new_image_signal.emit()
                

                # cv2.namedWindow("title", cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)  # allow window resize (Linux)
                # cv2.resizeWindow("title", 640, 480)
                # cv2.imshow("title", image)
                # cv2.waitKey(100)  # 1 millisecond

        



    





# img = cv2.imread("image.raw" )

# cv2.imshow("Display window", img)
# k = cv2.waitKey(0)