import cv2 
import numpy as np
import math

def nearest(img,scale):
    height,width=img.shape[:2]
    new_height=int(height*scale)
    new_width=int(width*scale)
    new_img = np.zeros((new_height,new_width,3),dtype=np.uint8)
    for k in range(3):
        for i in range(new_height):
            for j in range(new_width):
                x=min(round(i/scale),height-1)
                y=min(round(j/scale),width-1)
                new_img[i,j,k]=img[x,y,k]
    return new_img

def double_liner(img,scale):
    height,width = img.shape[:2]
    new_height = int(height * scale)
    new_width = int(width * scale)
    new_img = np.zeros((new_height,new_width,3),dtype=np.uint8)
    for k in range(3):
        for i in range(new_height):
            for j in range(new_width):
                x,y = float((i )/ scale),float((j )/scale)
                x0,y0 = int(math.floor(x)), int(math.floor(y))
                x1,y1 = min(x0+1,height-1),min(y0+1,width-1)
                dx = x-x0
                dy = y-y0
                value_up = (1-dx)*img[x0,y0,k] + dx*img[x1,y0,k]
                value_down = (1-dx)*img[x0,y1,k] + dx*img[x1,y1,k]
                new_img[i,j,k] = (1-dy)*value_up + dy * value_down
    return new_img


if __name__ == '__main__':
    cat_img = cv2.imread('cat.png',1)
    cat_nearest = nearest(cat_img,0.5)
    cat_double = double_liner(cat_img,0.5)

    cv2.imshow('cat_img',cat_img)
    cv2.imshow('cat_nearest',cat_nearest)
    cv2.imshow('cat_double',cat_double)

    while True:
        key = cv2.waitKey(0)
        if key == ord('q'):
            break
    cv2.destroyAllWindows()