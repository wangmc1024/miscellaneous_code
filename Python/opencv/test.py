import cv2

bgr_cat = cv2.imread("cat.png")
hsv_cat = cv2.cvtColor(bgr_cat, cv2.COLOR_BGR2HSV)
h,s,v = cv2.split(hsv_cat)
v[:,:] = 255
new_hsv_cat = cv2.merge([h,s,v])
new_bgr_cat = cv2.cvtColor(new_hsv_cat, cv2.COLOR_HSV2BGR)
cv2.imshow("cat", new_bgr_cat)
cv2.waitKey(0)
cv2.destroyAllWindows()