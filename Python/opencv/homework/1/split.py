import cv2
import os

def split(img,rows,cols,output_dir):
    height,width = img.shape[:2]

    one_height = height // rows
    one_width = width // cols

    for i in range(rows):
        for j in range(cols):
            new_img = img[i*one_height:i*one_height+one_height,j*one_width:j*one_width+one_width]

            if output_dir:
                os.makedirs(output_dir,exist_ok=True)
                name = ""
                if i == 0 and j == 0:
                    name = "2_e.png"
                elif i == 0 and j == 1:
                    name = "2_f.png"
                elif i == 1 and j == 0:
                    name = "2_g.png"
                elif i == 1 and j == 1:
                    name = "2_h.png"

                # cv2.imshow(name,new_img)
                # cv2.waitKey(1)
                # if input("Do you want to save the image? (y/n) ") == "y":
                #     cv2.destroyWindow(name)
                cv2.imwrite(os.path.join(output_dir,name),new_img)
                print("Image saved to",os.path.join(output_dir,name))
                # else:
                #     print("Image not saved")

if __name__ == "__main__":
    img = cv2.imread("image2.png")
    split(img,2,2,".")


