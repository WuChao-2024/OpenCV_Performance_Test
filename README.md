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
cmake \
-D PNG_LIBRARY_RELEASE=/usr/lib/aarch64-linux-gnu/libpng.so \
../

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