import cv2
import numpy as np

cat = cv2.imread('cat.png',0)
min = 100
max = 200
img = cv2.inRange(cat, min, max)
cv2.imshow('cat', img)

cv2.waitKey()
cv2.derstroyAllWindows()