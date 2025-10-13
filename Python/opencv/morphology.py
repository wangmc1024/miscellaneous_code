import cv2
import numpy as np
import matplotlib.pyplot as plt

origin_cat = cv2.imread('cat.png',0)

#kernel = cv2.getStructuringElement(shape,ksize[,anchor])
#shape代表形状类型
#cv2.MORPH_RECT
#cv2.MORPH_CROSS
#cv2.MORPH_ELLIPSE
kernel = np.ones((5,5),np.uint8)
erode_cat = cv2.erode(origin_cat,kernel)

#iterations 表示执行次数,默认为1
dilate_cat = cv2.dilate(erode_cat,kernel,iterations=1)

#cv2.MORPH_ERODE 腐蚀 cv2.MORPH_DILATE 膨胀
#cv2.MORPH_OPEN 开运算 cv2.MORPH_CLOSE 闭运算
#cv2.MORPH_TOPHAT 顶帽运算 cv2.MORPH_BLACKHAT 黑帽运算
#cv2.MORPH_GRADIENT 形态学梯度运算
#cv2.MORPH_HITMISS 击中不击中

open_cat=cv2.morphologyEx(origin_cat,cv2.MORPH_OPEN,kernel=kernel)
gradient_cat=cv2.morphologyEx(origin_cat,cv2.MORPH_GRADIENT,kernel=kernel)
tophat_cat=cv2.morphologyEx(origin_cat,cv2.MORPH_TOPHAT,kernel=kernel)

cv2.imshow("new",gradient_cat)
titles = ["origin_cat","erode_cat","dilate_cat","open_cat","gradient_cat","tophat_cat"]

for i in range(titles.__len__()):
    plt.subplot(3,3,i+1)
    plt.imshow(eval(titles[i]),cmap="gray")
    plt.title(titles[i])
    plt.axis("off")

plt.tight_layout()
plt.show()


# while True:
#     key = cv2.waitKey(100)
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows()