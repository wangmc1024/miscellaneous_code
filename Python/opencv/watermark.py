import cv2
import numpy as np
watermark = cv2.imread('watermark.png',0)
cat = cv2.imread("cat.png",0)

mask = watermark[:,:]>0
watermark[mask] = 1
cat = (cat>>1)<<1
cat += watermark
cv2.imshow("cat", cat)

mid = np.ones(cat.shape, dtype=np.uint8)*1
origin_mask = cv2.bitwise_and(cat, mid)
origin_mask[origin_mask>0] = 255
cv2.imshow("origin_mask", origin_mask)

cv2.waitKey(0)  
cv2.destroyAllWindows()
