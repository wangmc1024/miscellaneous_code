import cv2

bottle = cv2.imread("1_1.png")
sky = cv2.imread("1_2.png")

b_len,b_width = bottle.shape[:2]
sky = cv2.resize(sky,(b_width,b_len))

result = cv2.addWeighted(bottle,0.9,sky,0.1,0)

cv2.imshow("result",result)
while True:
    # 等待100毫秒获取按键（返回值为ASCII码，若无按键则为-1）
    key = cv2.waitKey(100) & 0xFF  # 按位与操作确保在不同系统上兼容
    
    # 若按下 'q' 键（ASCII码为113），则退出循环
    if key == ord('q'):
        break
    # 其他按键按下后不做处理，窗口继续保持

# 关闭所有窗口
cv2.destroyAllWindows()