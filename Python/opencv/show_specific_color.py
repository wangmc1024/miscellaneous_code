import cv2
import numpy as np

#H:hue,色调
#S:saturation,饱和度
#V:value,亮度

#先将BGR图片转换为HSV图片
#在利用inRange函数提取出对应的颜色范围的图片
#将提取出来的值作为蒙版(掩膜),与原图进行按位与运算,得到特定颜色的图片
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
