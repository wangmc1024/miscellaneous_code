'''
仿射变换利用2x3的矩阵来实现对应目的
变换过后平行线依旧是平行线
'''
import cv2
import numpy as np
cat = cv2.imread('cat.png')
cv2.imshow("cat",cat)

rows,cols = cat.shape[:2]
# new_cat = cv2.resize(cat,(2*cols,2*rows))

#new_cat = cv2.flip(cat,1) # 0垂直翻转，1水平翻转，-1水平垂直翻转

# M = np.float32([[1.0,0.0,100.0],[0.0,1.0,50.0]]) #平移矩阵

#new_cat = cv2.rotate(cat,cv2.ROTATE_90_CLOCKWISE) # 旋转90度

#Matrix = cv2.getRotationMatrix2D(start_point,angle,scale)
#start_point:旋转中心点
#angle:旋转角度
#scale:缩放因子
# M = cv2.getRotationMatrix2D((rows/2,cols/2),45,0.6)

origin = np.float32([[0,0],[0,rows-1],[cols-1,0]])
dst = np.float32([[0,rows*0.33],[cols*0.15,rows*0.7],[cols*0.85,rows*0.25]])
M = cv2.getAffineTransform(origin,dst)
new_cat = cv2.warpAffine(cat,M,(cols,rows))


cv2.imshow("new_cat",new_cat)
cv2.waitKey(0)
cv2.destroyAllWindows()