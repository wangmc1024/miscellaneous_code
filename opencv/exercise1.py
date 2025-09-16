import cv2
import matplotlib.pyplot as plt
import numpy as np

%matplotlib inline

img = cv2.imread('image.jpg')

cv2.imshow('image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()