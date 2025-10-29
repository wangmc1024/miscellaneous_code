import cv2
import numpy as np


def barrel_correction_improved(
    src_image, 
    param_a=0.08, 
    param_b=-0.36, 
    param_c=0.0, 
    param_d=None,
    scale_factor=1.2,  # 缩放因子，扩大图像后再校正
    interpolation=cv2.INTER_LANCZOS4  # 使用更高级的插值算法
):

    if src_image is None or src_image.size == 0:
        raise ValueError("输入图像为空或无效")
    
    h, w = src_image.shape[:2]
    
    # 步骤1：按比例放大图像，为边缘校正提供更多像素信息
    new_w = int(w * scale_factor)
    new_h = int(h * scale_factor)
    scaled_image = cv2.resize(src_image, (new_w, new_h), interpolation=cv2.INTER_LANCZOS4)
    
    # 步骤2：在放大后的图像上进行畸变校正
    h_scaled, w_scaled = scaled_image.shape[:2]
    xcen = (w_scaled - 1.0) / 2.0
    ycen = (h_scaled - 1.0) / 2.0
    norm_dist = min(xcen, ycen)
    
    if param_d is None:
        param_d = 1.0 - param_a - param_b - param_c
    
    # 生成坐标网格
    x, y = np.meshgrid(np.arange(w_scaled), np.arange(h_scaled))
    x = x.astype(np.float32)
    y = y.astype(np.float32)
    
    # 计算归一化偏移量
    xoff = (x - xcen) / norm_dist
    yoff = (y - ycen) / norm_dist
    
    # 计算半径映射
    r_dest = np.sqrt(xoff**2 + yoff**2)
    r_src = param_a * r_dest**4 + param_b * r_dest**3 + param_c * r_dest**2 + param_d * r_dest
    
    # 计算源坐标
    angle = np.arctan2(yoff, xoff)
    x_src = xcen + r_src * norm_dist * np.cos(angle)
    y_src = ycen + r_src * norm_dist * np.sin(angle)
    
    # 修复：确保映射矩阵的数据类型正确
    x_src = x_src.astype(np.float32)
    y_src = y_src.astype(np.float32)
    
    # 使用更高级的插值算法
    corrected_scaled = cv2.remap(
        scaled_image,
        x_src,
        y_src,
        interpolation=interpolation,
        borderMode=cv2.BORDER_REFLECT_101,  # 使用边缘反射填充，减少边界模糊
        borderValue=0
    )
    
    # 步骤3：裁剪回原始尺寸，去除边缘模糊区域
    start_x = int((new_w - w) / 2)
    start_y = int((new_h - h) / 2)
    corrected_image = corrected_scaled[start_y:start_y+h, start_x:start_x+w]
    
    return corrected_image


def adaptive_barrel_correction(src_image, edge_preservation=0.8):
    h, w = src_image.shape[:2]
    
    # 分析图像边缘信息，计算合适的校正参数
    gray = cv2.cvtColor(src_image, cv2.COLOR_BGR2GRAY)
    edges = cv2.Canny(gray, 50, 150)
    
    # 统计边缘像素分布
    edge_density = np.sum(edges) / (h * w * 255)
    
    # 根据边缘密度自适应调整校正参数
    base_param_a = 0.08
    base_param_b = -0.36
    
    # 边缘区域保留更多原始信息
    param_a = base_param_a * (1 - edge_preservation * edge_density)
    param_b = base_param_b * (1 - edge_preservation * edge_density)
    
    print(f"自适应参数调整 - param_a: {param_a:.4f}, param_b: {param_b:.4f}")
    
    return barrel_correction_improved(
        src_image,
        param_a=param_a,
        param_b=param_b,
        scale_factor=1.3,  # 更大的缩放因子
        interpolation=cv2.INTER_LANCZOS4
    )


if __name__ == "__main__":

    original_image = cv2.imread("image.png")
    
    corrected_image = adaptive_barrel_correction(original_image, edge_preservation=0.7)

    cv2.imshow("origin", original_image)
    cv2.imshow("correct", corrected_image)
    while True:
        key = cv2.waitKey(0)
        if key == ord('q'):
            break 
    cv2.destroyAllWindows()
    
   
