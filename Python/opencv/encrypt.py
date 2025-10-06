import cv2
import numpy as np

cat = cv2.imread('image.png')
password = np.random.randint(0,255,cat.shape,np.uint8)
cv2.imshow("password",password)

after_cat = cv2.bitwise_xor(cat,password)
cv2.imshow("after_cat",after_cat)

origin_cat = cv2.bitwise_xor(after_cat,password)
cv2.imshow("origin_cat",origin_cat)

cv2.waitKey()
cv2.destroyAllWindows()