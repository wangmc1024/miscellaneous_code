import cv2
import numpy as np
import matplotlib.pyplot as plt

def laplacian(img):
    img_lap = cv2.Laplacian(img, cv2.CV_64F)
    img_lap = cv2.convertScaleAbs(img_lap)
    return img_lap

def sobel(img):
    img_x = cv2.Sobel(img, cv2.CV_64F, 1, 0)
    img_y = cv2.Sobel(img, cv2.CV_64F, 0, 1)
    img_x = cv2.convertScaleAbs(img_x)
    img_y = cv2.convertScaleAbs(img_y)
    return cv2.addWeighted(img_x, 0.5, img_y, 0.5, 0)

def mean_filter(img):
    return cv2.blur(img, (5, 5))

def multiply(img1, img2):
    img1_normal = img1 / 255
    img2_normal = img2 / 255
    new_img = cv2.multiply(img1_normal, img2_normal)
    new_img = (new_img * 255).astype(np.uint8)
    return new_img

def power_law(img):
    gamma = 0.4
    c = 1
    img_normal = img / 255
    new_img = c*np.power(img_normal, gamma)
    new_img = (new_img * 255).astype(np.uint8)
    return new_img


if __name__ == '__main__':
    img_a = cv2.imread('2_a.png',0)

    img_b = laplacian(img_a)
    img_c = cv2.add(img_a, img_b)
    img_d = sobel(img_a)
    img_e = mean_filter(img_d)
    img_f = multiply(img_c, img_e)
    img_g = cv2.add(img_a, img_f)
    img_h = power_law(img_g)

    titles = ['img_a','img_b','img_c','img_d','img_e','img_f','img_g','img_h']
    plt.figure("result",figsize=(12, 8))

    for i in range(titles.__len__()):
        plt.subplot(2, 4, i + 1)
        plt.imshow(eval(titles[i]), cmap='gray')
        plt.title(titles[i])
    plt.tight_layout()
    plt.show()