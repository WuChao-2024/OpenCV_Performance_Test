import numpy as np
import cv2, os, psutil
from time import time, sleep

runNum = 100     # 运行次数(计入总时间, 求平均)
preNum = 20   # 在总次数之前的预热次数(不计入总时间)
FPS_DATA = ""
MS_DATA = ""

# 控制台输出性能信息(传入的时间是秒)
def print_performance(operation, ave_seconds):
    global FPS_DATA, MS_DATA
    fps = 1.0 / ave_seconds
    average_time_ms = 1000*ave_seconds
    print("%-16sFPS: \033[31m%6.1f\033[0m, Time(ms): \033[31m%5.1f\033[0m"%(operation+",",fps,average_time_ms))
    FPS_DATA += "%6.1f\n"%fps
    MS_DATA += "%5.1f\n"%average_time_ms

# 高斯滤波
def guassian(img, size):
    total_time = 0.0
    guassian_kernelSize = (9, 9)
    for i in range(preNum): # 预热
        result = cv2.GaussianBlur(img, guassian_kernelSize, 0)
    for i in range(runNum):
        time_begin = time()
        result = cv2.GaussianBlur(img, guassian_kernelSize, 0)
        total_time += (time() - time_begin)
    print_performance("guassion", total_time / runNum)
    cv2.imwrite("./python_result/guassian_" + size + ".jpg", result)

# 均值滤波
def blur(img, size):
    total_time = 0.0
    guassian_kernelSize = (9, 9)
    for i in range(preNum): # 预热
        result = cv2.GaussianBlur(img, guassian_kernelSize, 0)
    for i in range(runNum):
        time_begin = time()
        result = cv2.blur(img, guassian_kernelSize, 0)
        total_time += (time() - time_begin)
    print_performance("blur", total_time / runNum)
    cv2.imwrite("./python_result/blur_" + size + ".jpg", result)

# resize_NEAREST，最近邻插值，最快的插值方法
def resize_NEAREST(img, size):
    total_time = 0.0
    target_size = (img.shape[1] // 2, img.shape[0] // 2)
    for i in range(preNum): # 预热
        result = cv2.resize(img, target_size, interpolation=cv2.INTER_NEAREST)
    for i in range(runNum):
        time_begin = time()
        result = cv2.resize(img, target_size, interpolation=cv2.INTER_NEAREST)
        total_time += (time() - time_begin)
    print_performance("resize(NEAREST)", total_time / runNum)
    cv2.imwrite("./python_result/resize(NEAREST)_" + size + ".jpg", result)

# resize_LINEAR，线性插值，默认的插值方法
def resize_LINEAR(img, size):
    total_time = 0.0
    target_size = (img.shape[1] // 2, img.shape[0] // 2)
    for i in range(preNum): # 预热
        result = cv2.resize(img, target_size, interpolation=cv2.INTER_LINEAR)
    for i in range(runNum):
        time_begin = time()
        result = cv2.resize(img, target_size, interpolation=cv2.INTER_LINEAR)
        total_time += (time() - time_begin)
    print_performance("resize(LINEAR)", total_time / runNum)
    cv2.imwrite("./python_result/resize(LINEAR)_" + size + ".jpg", result)

# rotate 旋转
def rotate(img, size):
    total_time = 0.0
    angle = 33.0
    center = (img.shape[1] // 2, img.shape[0] // 2)
    rotation_matrix = cv2.getRotationMatrix2D(center, angle, 1.0)
    for i in range(preNum): # 预热
        result = cv2.warpAffine(img, rotation_matrix, (img.shape[1], img.shape[0]))
    for i in range(runNum):
        time_begin = time()
        result = cv2.warpAffine(img, rotation_matrix, (img.shape[1], img.shape[0]))
        total_time += (time() - time_begin)
    print_performance("rotate", total_time / runNum)
    cv2.imwrite("./python_result/rotate_" + size + ".jpg", result)

# 畸变矫正
def correct(img, size):
    # 相机内参
    camera_matrix = np.array([
        [762.111249, 0.000000, 994.975503], # 焦距
        [0.000000, 763.725775, 585.178879], # 主点坐标
        [0.000000, 0.000000, 1.000000]],  # 比例因子
        dtype=np.float32)
    # 畸变系数
    distortion_coeffs = np.array([
        0.051019, -0.046566,  # k1, k2,径向畸变系数，通常用于描述由于镜头非球面造成的图像失真。
        0.010455, 0.007456, 0.000000] # p1, p2,切向畸变系数，通常用于描述由于镜头与图像传感器不平行造成的图像失真。
        , dtype=np.float32)   # 省略了k4, k5, k6
    total_time = 0.0
    for i in range(preNum): # 预热
        result = cv2.undistort(img, camera_matrix, distortion_coeffs)
    for i in range(runNum):
        time_begin = time()
        result = cv2.undistort(img, camera_matrix, distortion_coeffs)
        total_time += (time() - time_begin)
    print_performance("correct", total_time / runNum)
    cv2.imwrite("./python_result/correct_" + size + ".jpg", result)

# 边缘检测
def canny(img, size):
    total_time = 0.0
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    for i in range(preNum): # 预热
        result = cv2.Canny(gray, 50, 150)
    for i in range(runNum):
        time_begin = time()
        result = cv2.Canny(gray, 50, 150)
        total_time += (time() - time_begin)
    print_performance("canny", total_time / runNum)
    cv2.imwrite("./python_result/canny_" + size + ".jpg", result)

# 人脸检测
def face_detect(img, size):
    faceCascade = cv2.CascadeClassifier("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")
    if not faceCascade:
        print("Error: Could not load face cascade.")
        return
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    face_detect_result = img.copy()
    total_time = 0.0
    for i in range(preNum): # 预热
        faces = faceCascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=3, minSize=(100, 100))
    for i in range(runNum):
        time_begin = time()
        faces = faceCascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=3, minSize=(100, 100))
        total_time += (time() - time_begin)
    for face in faces:
        cv2.rectangle(face_detect_result, tuple(face), (0, 255, 0), 2)
    print_performance("face_detect", total_time / runNum)
    cv2.imwrite("./python_result/face_detect_" + size + ".jpg", face_detect_result)

def main():
    image_512 = cv2.imread("./images/512w512h_raw.jpg")
    image_983 = cv2.imread("./images/983w553h_raw.jpg")
    image_1280 = cv2.imread("./images/1280w720h_resize.jpg")
    image_1920 = cv2.imread("./images/1920w1080h_resize.jpg")
    image_2560 = cv2.imread("./images/2560w1440h_resize.jpg")
    image_3840 = cv2.imread("./images/3840w2160h_resize.jpg")
    image_correct = cv2.imread("./images/correct_1920x1080.jpeg")

    images = [image_512, image_983, image_1280, image_1920, image_2560, image_3840]
    sizes = ["512x512", "983x553", "1280x720", "1920x1080", "2560x1440", "3840x2160"]

    if not os.path.exists("./python_result"):
        os.mkdir("./python_result")

    # 获取当前进程pid，用于设置CPU亲和性
    p = psutil.Process()
    try:
        p.cpu_affinity([])
        print(f"\n\033[1;32;41m Process {p.pid} bound to CPU core {p.cpu_affinity()}.\033[0m")
    except AttributeError:
        print("\n\033[1;32;41m Error: Unable to set CPU affinity\033[0m")
        exit()
    # 记录输出数据到文本文件
    global FPS_DATA, MS_DATA
    FPS_DATA += "00_cpp_test_all_core.cpp\n FPS DATA\n"
    MS_DATA += "00_cpp_test_all_core.cpp\n MS DATA\n"
    for image, size in zip(images, sizes):
        print(f"\n\033[32m----------------- {size}(default) -----------------\033[0m")
        guassian(image, size)
        blur(image, size)
        resize_LINEAR(image, size)
        resize_NEAREST(image, size)
        rotate(image, size)
        canny(image, size)
        correct(image, size)
        face_detect(image, size)
    # 按照测试顺序保存到本地
    saveData = FPS_DATA + MS_DATA
    try:
        with open('./log_02_py_test_all_core.txt', 'w') as f:
            # 将字符串写入文件
            f.write(saveData)
        print("\n\033[1;32;41m 保存成功！ \033[0m")
    except:
        print("\n\033[1;32;41m 保存失败！ \033[0m")

if __name__ == "__main__":
    main()