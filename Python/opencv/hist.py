import cv2
import matplotlib.pyplot as plt

cat = cv2.imread("cat.png",0)

#hist = cv2.calcHist(images,channels,mask,histSize,ranges,accumulate)
#histSize: number of bins in the histogram
#ranges: range of pixel values,left closed, right open
#accumulate: accumulate the histogram or not
# histb = cv2.calcHist([cat],[0],None,[256],[0,256])
# histg = cv2.calcHist([cat],[1],None,[256],[0,256])
# histr = cv2.calcHist([cat],[2],None,[256],[0,256])

# plt.plot(histb,color='b')
# plt.plot(histg,color='g')
# plt.plot(histr,color='r')

cv2.imshow("cat",cat)
equal_cat = cv2.equalizeHist(cat)
cv2.imshow("equal_cat",equal_cat)

# plt.figure("原始图像")
# plt.hist(cat.ravel(),255,color='b')
# plt.figure("均衡化图像")
# plt.hist(equal_cat.ravel(),255,color='b')

plt.figure("sbplot实例")
plt.subplot(1,2,1)
plt.hist(cat.ravel(),255,color='b')

plt.subplot(1,2,2)
plt.hist(equal_cat.ravel(),255,color='b')
plt.show()
cv2.waitKey()
cv2.destroyAllWindows()