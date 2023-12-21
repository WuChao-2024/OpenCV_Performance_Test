import cv2, os
import numpy as np
from time import time
from py00_ConfidenceInterval import ConfidenceInterval # 计算均值和置信区间半宽度
mother_path = os.path.join("img")  # 测试图片放在"./img/"路径下
for name in os.listdir(mother_path):
    if name.endswith(".jpg"):
        RUN_NUM = 100
        ## 测时(浅拷贝)
        time_list = []
        img = cv2.imread(os.path.join("img", name))
        for i in range(int(RUN_NUM*0.2)): # 小跑一会让数值稳定
            height, width = img.shape[:2]       # 获取图像的宽度和高度
            start_row,end_row,start_col,end_col = height//4, height*3//4, width//4, width*3//4
            cropped_img = img[start_row:end_row, start_col:end_col] # 裁剪图像并浅拷贝
        for i in range(RUN_NUM):
            time_begin = time()
            height, width = img.shape[:2]       # 获取图像的宽度和高度
            start_row,end_row,start_col,end_col = height//4, height*3//4, width//4, width*3//4
            cropped_img = img[start_row:end_row, start_col:end_col] # 裁剪图像并浅拷贝
            time_list.append(time() - time_begin)
        # 计算95%置信区间的半宽度(对于正态分布，使用1.96作为Z-score)
        mean, half_width = ConfidenceInterval(time_list, 1.96)
        # 输出结果
        print("%-22s,\033[31m%7.2f ± %4.2f ms\033[0m(浅拷贝), "%(name, 1000*mean,1000*half_width),end="")
        
        ## 测时(深拷贝)
        time_list = []
        img = cv2.imread(os.path.join("img", name))
        for i in range(int(RUN_NUM*0.2)): # 小跑一会让数值稳定
            height, width = img.shape[:2]       # 获取图像的宽度和高度
            start_row,end_row,start_col,end_col = height//4, height*3//4, width//4, width*3//4
            cropped_img = img[start_row:end_row, start_col:end_col].copy() # 裁剪图像并深拷贝
        for i in range(RUN_NUM):
            time_begin = time()
            height, width = img.shape[:2]       # 获取图像的宽度和高度
            start_row,end_row,start_col,end_col = height//4, height*3//4, width//4, width*3//4
            cropped_img = img[start_row:end_row, start_col:end_col].copy() # 裁剪图像并深拷贝
            time_list.append(time() - time_begin)
        # 计算95%置信区间的半宽度(对于正态分布，使用1.96作为Z-score)
        mean, half_width = ConfidenceInterval(time_list, 1.96)
        # 输出结果
        print("\033[31m%7.2f ± %4.2f ms\033[0m(深拷贝), P=95%%"%(1000*mean,1000*half_width))