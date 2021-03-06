import pyfirmata as f
from threading import Timer
import numpy as np
import imutils
import pickle
import cv2
import os

board = f.Arduino('/dev/ttyACM0')

board.digital[12].mode = f.INPUT
board.digital[14].mode = f.INPUT
actuatorw1 = board.get_pin('d:12:o')
actuatorw2 = board.get_pin('d:14:o')


class Door:
    def door_lock():
        actuatorw1.write(1)
        actuatorw1.write(0)
        Timer(2.0, actuator_off).start()

    def door_unlock():
        actuatorw1.write(0)
        actuatorw1.write(1)
        Timer(10.0, door_lock).start()

    def actuator_off():
        actuatorw1.write(0)
        actuatorw1.write(0)

def face_detect(detector, embedding_model, recognizer, le):
    protoPath = os.path.sep.join([detector, "deploy.prototxt"])
    modelPath = os.path.sep.join([detector,
	    "res10_300x300_ssd_iter_140000.caffemodel"])
    fdetector = cv2.dnn.readNetFromCaffe(protoPath, modelPath)
    fembedder = cv2.dnn.readNetFromTorch(embedding_model)
    frecognizer = pickle.loads(open(recognizer, "rb").read())
    fle = pickle.loads(open(le, "rb").read())