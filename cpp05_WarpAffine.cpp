import cv2, os
from time import time
mother_path = os.path.join("img")  # 测试图片放在"./img/"路径下
for name in os.listdir(mother_path):
    if name.endswith(".jpg"):
        img = cv2.imread(os.path.join("img", name))
        RUN_NUM = 30
        # 线性插值
        time_total = 0.0
        for i in range(RUN_NUM):
            time_begin = time()
            height, width = img.shape[:2]       # 获取图像的宽度和高度
            center = (width // 2, height // 2)  # 旋转中心选择图像的中心点
            rotated_img = cv2.warpAffine(img,
                                        cv2.getRotationMatrix2D(center, 33, 1), 
                                        (width, height),
                                        flags=cv2.INTER_LINEAR
                                        )
            time_total += (time() - time_begin)
        print("%-22s,\033[31m%7.2f ms\033[0m(线性插值)"%(name, 1000*time_total/RUN_NUM), end=" ")
        # 最近邻插值
        time_total = 0.0
        for i in range(RUN_NUM):
            time_begin = time()
            height, width = img.shape[:2]       # 获取图像的宽度和高度
            center = (width // 2, height // 2)  # 旋转中心选择图像的中心点
            rotated_img = cv2.warpAffine(img,
                                        cv2.getRotationMatrix2D(center, 33, 1), 
                                        (width, height),
                                        flags=cv2.INTER_NEAREST
                                        )
            time_total += (time() - time_begin)
        print("\033[31m%7.2f ms\033[0m(最近邻插值)"%(1000*time_total/RUN_NUM))