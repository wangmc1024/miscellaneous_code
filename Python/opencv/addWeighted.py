import cv2

cat = cv2.imread("cat.png")
lion = cv2.imread("lion.png")

#cv2.addWeighted(src1,alpha,src2,beta,gamma)
#alpha and beta are the weight of the two images respectively
#gamma is the scalar added to each sum
#result = (src1*alpha) + (src2*beta) + gamma
new_im = cv2.addWeighted(lion,0.1,cat,0.9,0)

cv2.imshow("new",new_im)
cv2.waitKey()
cv2.destroyAllWindows()
