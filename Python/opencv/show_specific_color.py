import cv2
import numpy as np

def show_specific_color(bgr_image, hsv_image, color):
    min = np.array([color-10, 0, 0])
    max = np.array([color+10, 255, 255])

    mask = cv2.inRange(hsv_image, min, max)
    specific_img = cv2.bitwise_and(bgr_image, bgr_image, mask=mask)
    cv2.imshow(color.__str__(), specific_img)
    cv2.waitKey(0)
    cv2.destroyAllWindows() 

brg_cat = cv2.imread("image.png")
if brg_cat is None:
    print("Error: Image not found.")
    exit()

hsv_cat = cv2.cvtColor(brg_cat, cv2.COLOR_BGR2HSV)
show_specific_color(brg_cat, hsv_cat, 10)
show_specific_color(brg_cat, hsv_cat, 60)
show_specific_color(brg_cat, hsv_cat, 120)
