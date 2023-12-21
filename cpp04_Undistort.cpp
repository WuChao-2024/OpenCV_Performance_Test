import cv2, os
import numpy as np
from time import time
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
        img = cv2.imread(os.path.join("img", name))
        kernel_size = 9
        RUN_NUM = 30
        time_total = 0.0
        for i in range(RUN_NUM):
            time_begin = time()
            cv2.undistort(img, camera_matrix, distortion_coeffs)
            time_total += (time() - time_begin)
        print("%-22s,\033[31m%7.2f ms\033[0m"%(name, 1000*time_total/RUN_NUM))

