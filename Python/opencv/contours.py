import cv2
import matplotlib.pyplot as plt
import numpy as np

cat = cv2.imread('lion.png',0)

th,new = cv2.threshold(cat,0,255,cv2.THRESH_BINARY_INV+cv2.THRESH_OTSU)
cv2.imshow("threshold",new)

contours,hierarchy = cv2.findContours(new,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
mask = np.zeros(cat.shape,np.uint8)
mask = cv2.drawContours(mask,contours,-1,(255,255,255),-1)
cv2.imshow("result",mask)
cv2.waitKey()
cv2.destroyAllWindows()