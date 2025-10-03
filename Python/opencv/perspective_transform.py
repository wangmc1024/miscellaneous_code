import cv2
import numpy as np

cat = cv2.imread("cat.png")
rows,cols = cat.shape[:2]
pts1 = np.float32([[230,60],[200,300],[500,100],[150,300]])
pts2 = np.float32([[50,50],[500,50],[50,500],[500,500]])
M = cv2.getPerspectiveTransform(pts1,pts2)
new_cat = cv2.warpPerspective(cat,M,(cols,rows))
cv2.imshow("cat",new_cat)
cv2.waitKey()
cv2.destroyAllWindows()