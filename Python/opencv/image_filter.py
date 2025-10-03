import cv2
import time

cat = cv2.imread("cat.png")
#dst = cv2.blur(cat,(3,3))  均值滤波
dst = cv2.boxFilter(cat,cv2.CV_16U,(10,10),normalize=0) #方框滤波
# dst = cv2.GaussianBlur(cat,(5,5),0.1,0.1) 高斯滤波
# dst = cv2.medianBlur(cat,3)   中值滤波
# kernel = np.ones((9,9),np.float32)/81
# dst = cv2.filter2D(cat,-1,kernel)
start = time.time()
#dst = cv2.bilateralFilter(cat,-1,100,1000) #双边滤波
end = time.time()
print(f"run time:{end-start:.2f}s")
# test = dst + 100
cv2.imshow("cat",cat)
cv2.imshow("new",dst)
# cv2.imshow("test",test)
cv2.waitKey()
cv2.destroyAllWindows()