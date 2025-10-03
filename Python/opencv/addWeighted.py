import cv2

cat = cv2.imread("cat.png")
lion = cv2.imread("lion.png")

new_im = cv2.addWeighted(lion,0.1,cat,0.9,0)

cv2.imshow("new",new_im)
cv2.waitKey()
cv2.destroyAllWindows()
