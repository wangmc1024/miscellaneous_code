import cv2
import numpy as np
'''
cv2.THRESH_BINARY  二值化阈值处理
cv2.THRESH_BINARY_INV
cv2.THRESH_TRUNC    截断阈值处理（超过阈值的部分截断成阈值）
cv2.THRESH_TOZERO_INV   超阈值零处理
cv2.THRESH_TOZERO       
'''

cat = cv2.imread('cat.png',0)
cv2.imshow("cat",cat)
#thresh,image = cv2.threshold(image,thresh_value,max_value,threshold_type)
#max_value 只对cv2.THRESH_BINARY和cv2.THRESH_BINARY_INV有用，表示二值化的最大值
#th,new = cv2.threshold(cat,128,255,cv2.THRESH_BINARY)
#th,new = cv2.threshold(cat,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU) #OSTU处理

#image = cv2.adaptivedThreshold(image,max_value,cv2.ADAPTIVE_THRESH_MEAN_C,threshold_type,block_size,C)
#cv2.ADAPTIVE_THRESH_MEAN_C  均值阈值处理
#cv2.ADAPTIVE_THRESH_GAUSSIAN_C  高斯阈值处理
#block_size  邻域大小,越大越模糊,必须为奇数,一般为3,5,7
#C  常数值,越大越平滑,一般为10,20,减去均值

new = cv2.adaptiveThreshold(cat,255,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,5,3)
new1 = cv2.adaptiveThreshold(cat,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,5,3)
#print(th)
cv2.imshow("mean",new)
cv2.imshow("gaussian",new1)
cv2.waitKey()
cv2.destroyAllWindows()