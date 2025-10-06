import cv2
import time

cat = cv2.imread("cat.png")
#dst = cv2.blur(cat,(3,3))  均值模糊


#image = cv2.boxFilter(image, ddepth, ksize, anchor, normalize, borderType)
#ddepth：输出图像的深度，可以为-1、CV_8U、CV_16U、CV_16S、CV_32F、CV_64F。
#ksize：卷积核的大小，必须是奇数，如(3,3)、(5,5)等。
#anchor：锚点，默认为(-1,-1)，表示锚点位于卷积核的中心。
#normalize：是否归一化，默认为0，表示不归一化。
#borderType：边界填充方式，默认为BORDER_DEFAULT，表示填充方式由具体实现定义。
dst = cv2.boxFilter(cat,cv2.CV_16U,(10,10),normalize=0) #方框滤波

#image = cv2.GaussianBlur(image,(ksize,ksize),sigmaX,sigmaY[,borderType])
#ksize：卷积核的大小，必须是奇数，如(3,3)、(5,5)等。
#sigmaX：X方向的标准差，如果为0，表示根据sigmaY自动计算。
#sigmaY：Y方向的标准差，如果为0，表示根据sigmaX自动计算。
#borderType：边界填充方式，默认为BORDER_DEFAULT，表示填充方式由具体实现定义。
# dst = cv2.GaussianBlur(cat,(5,5),0.1,0.1) 高斯模糊


# image = cv2.medianBlur(image,ksize)
# ksize：卷积核的大小，必须是奇数，如3、5等。
# dst = cv2.medianBlur(cat,3)   中值模糊

# image = cv2.filter2D(image,ddepth,kernel[,dst[,anchor[,delta[,borderType]]]])
# ddepth：输出图像的深度，可以为-1、CV_8U、CV_16U、CV_16S、CV_32F、CV_64F。
# kernel：卷积核，必须是3x3、5x5、7x7等大小的浮点型矩阵。
# kernel = np.ones((9,9),np.float32)/81
# dst = cv2.filter2D(cat,-1,kernel)


start = time.time()
#image = cv2.bilateralFilter(image,d,sigmaColor,sigmaSpace[,dst[,borderType]])
#d：用于计算空间距离的指数。如果为-1，表示根据sigmaSpace自动计算。如果为0，表示不使用空间距离。
#sigmaColor：颜色空间的标准差。值越大表示允许更多不同的颜色参与到计算中
#sigmaSpace：坐标空间的标准差。值越大表示允许更多更远的坐标参与到计算中
#dst = cv2.bilateralFilter(cat,-1,100,1000) #双边滤波
end = time.time()
print(f"run time:{end-start:.2f}s")
# test = dst + 100
cv2.imshow("cat",cat)
cv2.imshow("new",dst)
# cv2.imshow("test",test)
cv2.waitKey()
cv2.destroyAllWindows()