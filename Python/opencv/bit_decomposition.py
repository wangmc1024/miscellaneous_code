import cv2
import numpy as np

cat = cv2.imread("image.png")
length,width,chan = cat.shape

img1 = np.zeros((length,width,chan,8),np.uint8)
for j in range(3):
    for i in range(8):
        img1[:,:,j,i] = 2**i
ri = np.zeros((length,width,chan,8),np.uint8)
for j in range(3):
    for i in range(8):
        ri[:,:,j,i] = cv2.bitwise_and(img1[:,:,j,i],cat[:,:,j])
        mask = ri[:,:,j,i]>0
        ri[mask,j,i] = 255
for i in range(8):
    end = cv2.merge([ri[:,:,0,i],ri[:,:,1,i],ri[:,:,2,i]])
    cv2.imshow("img"+str(i),end)
cv2.waitKey(0)
cv2.destroyAllWindows()