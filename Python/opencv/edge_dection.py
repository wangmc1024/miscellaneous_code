'''
scharr算子比sobel算子更加平滑，但是速度更快，并且具有更高的灰度级精度
sobel算子可以同时在x和y方向上求导，而scharr只能在一个方向上求导
原理：在图像中，像素的梯度方向与图像强度的变化方向一致，
    即像素值变化最剧烈的地方与图像强度变化最剧烈的地方相邻
laplacian是进行全局处理
'''

import cv2
import matplotlib.pyplot as plt

cat = cv2.imread("cat.png",0)

cat_x_sobel = cv2.Sobel(cat,cv2.CV_64F,1,0)
cat_y_sobel = cv2.Sobel(cat,cv2.CV_64F,0,1)
cat_xy_sobel = cv2.Sobel(cat,cv2.CV_64F,1,1)
cat_x_sobel = cv2.convertScaleAbs(cat_x_sobel)
cat_y_sobel = cv2.convertScaleAbs(cat_y_sobel)
cat_xy_sobel = cv2.convertScaleAbs(cat_xy_sobel)
cat_x_y_sobel = cv2.addWeighted(cat_x_sobel,0.5,cat_y_sobel,0.5,0)

cat_x_scharr = cv2.Scharr(cat,cv2.CV_64F,1,0)
cat_y_scharr = cv2.Scharr(cat,cv2.CV_64F,0,1)
cat_x_scharr = cv2.convertScaleAbs(cat_x_scharr)
cat_y_scharr = cv2.convertScaleAbs(cat_y_scharr)
cat_x_y_scharr = cv2.addWeighted(cat_x_scharr,0.5,cat_y_scharr,0.5,0)

cat_laplacian = cv2.Laplacian(cat,cv2.CV_64F)
cat_laplacian = cv2.convertScaleAbs(cat_laplacian)

cat_canny = cv2.Canny(cat,100,200)

plt.figure("image")
tiles = ['cat_x_sobel','cat_y_sobel','cat_xy_sobel','cat_x_y_sobel','cat_x_scharr','cat_y_scharr','cat_x_y_scharr','cat_laplacian','cat_canny']

for i in range(tiles.__len__()):
    plt.subplot(3,3,i+1)
    plt.imshow(eval(tiles[i]),cmap='gray')
    plt.title(tiles[i])
    plt.axis('off')
plt.tight_layout()
plt.show()