import cv2
import matplotlib.pyplot as plt

cat = cv2.imread("cat.png")
histb = cv2.calcHist([cat],[0],None,[256],[0,255])
histg = cv2.calcHist([cat],[1],None,[256],[0,255])
histr = cv2.calcHist([cat],[2],None,[256],[0,255])

plt.plot(histb,color='b')
plt.plot(histg,color='g')
plt.plot(histr,color='r')
plt.show()
cv2.waitKey()
cv2.destroyAllWindows()