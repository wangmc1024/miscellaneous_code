import cv2
import numpy as np

imgBlue = np.zeros([100,100,3],np.uint8)
imgBlue[:,:,0] = 255
hsvBlue = cv2.cvtColor(imgBlue,cv2.COLOR_BGR2HSV)
hsvBlue = cv2.cvtColor(imgBlue,cv2.COLOR_HSV2BGR)
cv2.imshow("blue",hsvBlue)

imgGreen = np.zeros([100,100,3],np.uint8)
imgGreen[:,:,1] = 255
hsvGreen = cv2.cvtColor(imgGreen,cv2.COLOR_BGR2HSV)
hsvGreen = cv2.cvtColor(imgGreen,cv2.COLOR_HSV2BGR)
cv2.imshow("Green",hsvGreen)

imgRed = np.zeros([100,100,3],np.uint8)
imgRed[:,:,2] = 255
hsvRed = cv2.cvtColor(imgRed,cv2.COLOR_BGR2HSV)
hsvRed = cv2.cvtColor(imgRed,cv2.COLOR_HSV2BGR)
cv2.imshow("Red",hsvRed)


cv2.waitKey()
cv2.destroyAllWindows()