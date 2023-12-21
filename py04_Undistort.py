import cv2, os
import numpy as np
from time import time
from py00_ConfidenceInterval import ConfidenceInterval # 计算均值和置信区间半宽度
# 相机内参
camera_matrix = np.array([
    [458.654, 0, 367.215], # 焦距
    [0, 457.296, 248.375], # 主点坐标
    [0.0, 0.0, 1.0]],  # 比例因子
    dtype=np.float32)
# 畸变系数
distortion_coeffs = np.array([-4.28340811, 1.07395907,  # k1, k2,径向畸变系数，通常用于描述由于镜头非球面造成的图像失真。
                                0.00019359, 1.76187114e-05 # p1, p2,切向畸变系数，通常用于描述由于镜头与图像传感器不平行造成的图像失真。
                                ], dtype=np.float32)   # 省略了k3, k4, k5, k6
mother_path = os.path.join("img")  # 测试图片放在"./img/"路径下
for name in os.listdir(mother_path):
    if name.endswith(".jpg"):
        RUN_NUM = 100
        time_list = []
        # 测时
        img = cv2.imread(os.path.join("img", name))
        for i in range(int(RUN_NUM*0.2)): # 小跑一会让数值稳定
            new_img = cv2.undistort(img, camera_matrix, distortion_coeffs)
        for i in range(RUN_NUM):
            time_begin = time()
            new_img = cv2.undistort(img, camera_matrix, distortion_coeffs)
            time_list.append(time() - time_begin)
        # 计算95%置信区间的半宽度(对于正态分布，使用1.96作为Z-score)
        mean, half_width = ConfidenceInterval(time_list, 1.96)
        # 输出结果
        print("%-22s,\033[31m%7.2f ± %4.2f ms\033[0m, P=95%%"%(name, 1000*mean,1000*half_width))