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
3. 运行一键测试脚本
```bash
bash 01_python_test.sh
```
## OpenCV C++ 安装
1. 安装依赖
```bash
sudo apt-get install cmake
sudo apt-get install build-essential libgtk2.0-dev libavcodec-dev libavformat-dev libjpeg-dev libswscale-dev libtiff5-dev
sudo apt-get install libgtk2.0-dev
sudo apt-get install pkg-config
```
2. 下载OpenCV 4.8.0源码
```bash
wget https://github.com/opencv/opencv/archive/refs/tags/4.8.1.tar.gz
tar -xvf opencv-4.8.1.tar.gz
```
3. 编译OpenCV
```bash
cd opencv-4.8.1
mkdir build && cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE \
-D BUILD_opencv_dnn=OFF \
-D BUILD_libwebp=OFF \
-D BUILD_opencv_imgcodecs=OFF \
-D BUILD_opencv_gpu=OFF  \
-D BUILD_DOCS=OFF \
-D BUILD_PERF_TESTS=OFF \
-D BUILD_TESTS=OFF \
-D BUILD_WITH_DEBUG_INFO=OFF \
-D BUILD_opencv_apps=OFF \
-D BUILD_opencv_calib3d=OFF \
-D BUILD_opencv_contrib=OFF \
-D BUILD_opencv_features2d=OFF \
-D BUILD_opencv_flann=OFF \
-D BUILD_opencv_gpu=OFF \
-D BUILD_opencv_ml=OFF \
-D BUILD_opencv_nonfree=OFF \
-D BUILD_opencv_objdetect=OFF \
-D BUILD_opencv_photo=OFF \
-D BUILD_opencv_stitching=OFF \
-D BUILD_opencv_superres=OFF \
-D BUILD_opencv_ts=OFF \
-D BUILD_opencv_video=OFF \
-D BUILD_opencv_videostab=OFF \
-D CMAKE_INSTALL_PREFIX=/usr/local ../

make -j4
sudo make install
```
2. 编译完成后，在终端输入以下命令查看是否安装成功：
```bash
pkg-config --modversion opencv
```
如果显示版本号，说明安装成功。

# 编译和运行程序
```bash
g++ -o main main.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui
./main
```
# 运行结果
![运行结果]()