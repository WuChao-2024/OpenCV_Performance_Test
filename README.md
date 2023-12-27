# 说明
1. 此项目用于在嵌入式设备上测试OpenCV相关函数的性能，测试项目包括前处理常用功能和人脸检测的算法，C/C++代码和Python代码具有相同参数和功能。
2. 关于CPU亲和性：C++程序和Python程序均有绑定某一个核心运行的部分，和默认所有核心全开的部分，这在控制台输出内容均有体现。其中：绑定某一个核心运行的成绩能反映单核心的能力，OpenCV部分算法有并行处理，这部分成绩能反映整个SoC的能力。特别的，单核心部分RK3588需要单独测试A76大核心和A55小核心.
3. 在测试代码中充分考虑了缓存和预热：在第一次迭代中，CPU缓存可能还没有被充分填充，这可能导致首次迭代的时间较长。随着迭代次数的增加，CPU缓存的效果会更好，因此后续迭代的时间可能会更短。
4. OpenCV版本：4.8.1; Python版本：3.10.x; 
5. 参考测试图像:回眸女郎Lenna，通过原图裁剪，720p以上分辨率通过cv2.INTER_LANCZOS4插值方法放大而来。

# 使用方式
注：所需要的前置操作参考最后一章节
## Python部分
1. 运行Python脚本
```bash
sudo python3 02_py_test_all_core.py  && sudo python3 03_py_test_single_core.py 
```
2. 查看测试结果   
运行结束后，python程序生成的图片结果在```./python_result/```目录下。
## C/C++部分
1. 创建build编译目录
```bash
mkdir -p build && cd build
```
2. 编译
```bash
cmake .. && make -j2
```
3. 运行
```bash
./00_cpp_test_single_core && ./01_cpp_test_all_core 
```
4. 查看运行结果   
运行结束后，C/C++程序生成的图片结果在```./result_cpp/```目录下。       

# 参考指令
## 将设备设置为性能模式
1. Orange Pi 5
```bash
orangepi-config
```
在```system``` - ```CPU```中设置CPU最小频率为2400000
## 给Ubuntu更换阿里源
1. 查看ubuntu的Codename
```bash
lsb_release -a | grep Codename | awk '{print $2}' 
```
输出结果是```jammy```
2. 备份系统源
```bash
cd /etc/apt && sudo mv sources.list sources.list.bak
```
3. 替换系统源
```bash
vim sources.list
```
```bash
deb http://mirrors.aliyun.com/ubuntu/ jammy main multiverse restricted universe
deb http://mirrors.aliyun.com/ubuntu/ jammy-backports main multiverse restricted universe
deb http://mirrors.aliyun.com/ubuntu/ jammy-proposed main multiverse restricted universe
deb http://mirrors.aliyun.com/ubuntu/ jammy-security main multiverse restricted universe
deb http://mirrors.aliyun.com/ubuntu/ jammy-updates main multiverse restricted universe
deb-src http://mirrors.aliyun.com/ubuntu/ jammy main multiverse restricted universe
deb-src http://mirrors.aliyun.com/ubuntu/ jammy-backports main multiverse restricted universe
deb-src http://mirrors.aliyun.com/ubuntu/ jammy-proposed main multiverse restricted universe
deb-src http://mirrors.aliyun.com/ubuntu/ jammy-security main multiverse restricted universe
deb-src http://mirrors.aliyun.com/ubuntu/ jammy-updates main multiverse restricted universe
```
4. 更新索引
```bash
apt update
```
## OpenCV - Python 安装
1. 安装```pip```
```bash
apt install python3-pip
```
2. 安装 ```opencv-python``` 包
```bash
pip install opencv-python -i http://mirrors.aliyun.com/pypi/simple/ --trusted-host mirrors.aliyun.com
```
3. 安装 ```psutil``` 包
```bash
pip install psutil -i http://mirrors.aliyun.com/pypi/simple/ --trusted-host mirrors.aliyun.com
```

## OpenCV C++ 安装
1. 安装依赖
```bash
sudo apt-get install cmake build-essential libgtk2.0-dev libavcodec-dev libavformat-dev libjpeg-dev libpng-dev libswscale-dev libtiff5-dev libgtk2.0-dev pkg-config python3-distutils
```
2. 下载OpenCV 4.8.0源码
```bash
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
unzip opencv.zip
```
3. 编译OpenCV
```bash
cd opencv-4.x
mkdir -p build && cd build
```
```bash
cmake ..
```
香橙派使用以下指令
```bash
cmake \
-D PNG_LIBRARY_RELEASE=/usr/lib/aarch64-linux-gnu/libpng.so \
../

make -j8
sudo make install
```
