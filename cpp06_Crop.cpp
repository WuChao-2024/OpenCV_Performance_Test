import cv2, os
from time import time
mother_path = os.path.join("img")  # 测试图片放在"./img/"路径下
for name in os.listdir(mother_path):
    if name.endswith(".jpg"):
        img = cv2.imread(os.path.join("img", name))
        RUN_NUM = 30
        # 浅拷贝
        time_total = 0.0
        for i in range(RUN_NUM):
            time_begin = time()
            height, width = img.shape[:2]       # 获取图像的宽度和高度
            start_row,end_row,start_col,end_col = height//4, height*3//4, width//4, width*3//4
            cropped_img = img[start_row:end_row, start_col:end_col] # 裁剪图像并浅拷贝
            time_total += (time() - time_begin)
        print("%-22s,\033[31m%7.2f ms\033[0m(浅拷贝)"%(name, 1000*time_total/RUN_NUM), end=" ")
        # 深拷贝
        time_total = 0.0
        for i in range(RUN_NUM):
            time_begin = time()
            height, width = img.shape[:2]       # 获取图像的宽度和高度
            start_row,end_row,start_col,end_col = height//4, height*3//4, width//4, width*3//4
            cropped_img = img[start_row:end_row, start_col:end_col].copy() # 裁剪图像并深拷贝
            time_total += (time() - time_begin)
        print("\033[31m%7.2f ms\033[0m(深拷贝)"%(1000*time_total/RUN_NUM))