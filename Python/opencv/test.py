import cv2

cat = cv2.imread("image.png")
cv2.imshow("cat",cat)
b = cat[:,:,0]
g = cat[:,:,1]
r = cat[:,:,2]
cv2.imshow("blue",b)
cv2.imshow("green",g)
cv2.imshow("red",r)
new_cat = cv2.merge([b,g,r])
cv2.imshow("new",new_cat)
cv2.waitKey(0)
cv2.destroyAllWindows()