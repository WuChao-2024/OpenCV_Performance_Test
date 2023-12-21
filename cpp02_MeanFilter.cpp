import cv2, os
from time import time
mother_path = os.path.join("img")  # 测试图片放在"./img/"路径下
for name in os.listdir(mother_path):
    if name.endswith(".jpg"):
        img = cv2.imread(os.path.join("img", name))
        kernel_size = 9
        RUN_NUM = 30
        time_total = 0.0
        for i in range(RUN_NUM):
            time_begin = time()
            cv2.blur(img, (kernel_size, kernel_size))
            time_total += (time() - time_begin)
        print("%-22s,\033[31m%7.2f ms\033[0m"%(name, 1000*time_total/RUN_NUM))