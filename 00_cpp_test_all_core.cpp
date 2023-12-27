#include <opencv2/opencv.hpp>
#include <chrono>
#include <unistd.h>
#include <sched.h>
#include <filesystem>
#include <thread>
#include <sstream>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

static int runNum = 100;    // 运行次数(计入总时间, 求平均)
static int preNum = 20;     // 在总次数之前的预热次数(不计入总时间)
std::stringstream FPS_DATA;
std::stringstream MS_DATA;


//控制台输出性能信息(传入的时间是微秒)
void print_performance(const std::string& operation, double ave_microseconds) {
    double fps = 1000000.0 / ave_microseconds;
    double average_time_ms = ave_microseconds / 1000;
    // 操作的名称String左对齐，最小宽度16输出
    std::cout << std::left << std::setw(16) << operation + ","; 
    // FPS以红色字体，保留一位小数，最小宽度6输出
    std::cout << "FPS: \033[31m" << std::fixed << std::setprecision(1) << std::setw(6) << fps << "\033[0m";
    // 耗时的毫秒，以红色字体，保留一位小数，最小宽度5输出
    std::cout << ", Time(ms): \033[31m" << std::fixed << std::setprecision(1) << std::setw(5) << average_time_ms << "\033[0m" << std::endl;
    FPS_DATA << std::fixed << std::setprecision(1) << std::setw(6) << fps << "\n";
    MS_DATA << std::fixed << std::setprecision(1) << std::setw(5) << average_time_ms << "\n";
}

//高斯滤波
void guassian(const cv::Mat &image, const std::string &size){
    int total_time = 0;
    cv::Mat result;
    cv::Size guassian_kernelSize(9,9); 
    for(int i = 0 ; i < preNum ; i++)  // 预热
        cv::GaussianBlur(image, result, guassian_kernelSize, 0, 0);
    for(int i = 0 ; i < runNum ; i++){  // 实测
        auto tp_start = std::chrono::system_clock::now();
        cv::GaussianBlur(image, result, guassian_kernelSize, 0, 0);
        auto tp_end = std::chrono::system_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(tp_end - tp_start).count();
    }
    print_performance("guassion", total_time / runNum);
    cv::imwrite("../result_cpp/guassian_" + size + ".jpg", result);
}

//均值滤波
void blur(const cv::Mat &image, const std::string &size){
    int total_time = 0;
    cv::Mat result;
    cv::Size blur_kernelSize(9,9); 
    for(int i = 0 ; i < preNum ; i++)  // 预热
        cv::blur(image, result, blur_kernelSize);
    for(int i = 0 ; i < runNum ; i++){  // 实测
        auto tp_start = std::chrono::system_clock::now();
        cv::blur(image, result, blur_kernelSize);
        auto tp_end = std::chrono::system_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(tp_end - tp_start).count();
    }
    print_performance("blur", total_time / runNum);
    cv::imwrite("../result_cpp/blur_" + size + ".jpg",result);
}

//resize_NEAREST，最近邻插值，最快的插值方法
void resize_NEAREST(const cv::Mat &image, const std::string &size){
    int total_time = 0;
    cv::Mat reszie_result;
    cv::Size targetSize(image.cols / 2, image.rows / 2);
    for(int i = 0 ; i < preNum ; i++)  // 预热              
        cv::resize(image, reszie_result, targetSize,cv::INTER_NEAREST);
    for(int i = 0 ; i < runNum ; i++){  // 实测
        auto tp_start = std::chrono::system_clock::now();
        cv::resize(image, reszie_result, targetSize,cv::INTER_NEAREST);
        auto tp_end = std::chrono::system_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(tp_end - tp_start).count();
    }
    print_performance("resize(NEAREST)", total_time / runNum);
    cv::imwrite("../result/resize(NEAREST)_" + size + ".jpg",reszie_result);
}

//resize_LINEAR，线性插值，默认的插值方法
void resize_LINEAR(const cv::Mat &image, const std::string &size){
    int total_time = 0;
    cv::Mat reszie_result;
    cv::Size targetSize(image.cols / 2, image.rows / 2);
    for(int i = 0 ; i < preNum ; i++)  // 预热              
        cv::resize(image, reszie_result, targetSize,cv::INTER_LINEAR);
    for(int i = 0 ; i < runNum ; i++){  // 实测
        auto tp_start = std::chrono::system_clock::now();
        cv::resize(image, reszie_result, targetSize,cv::INTER_LINEAR);
        auto tp_end = std::chrono::system_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(tp_end - tp_start).count();
    }
    print_performance("resize(LINEAR)", total_time / runNum);
    cv::imwrite("../result/resize(LINEAR)_" + size + ".jpg",reszie_result);
}

//rotate 旋转
void rotate(const cv::Mat &image, const std::string &size){
    int total_time = 0;
    cv::Mat rotate_result;
    double angle = 33.0;
    cv::Point2f center(static_cast<float>(image.cols) / 2, static_cast<float>(image.rows) / 2);
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);
    for(int i = 0 ; i < preNum ; i++)  // 预热
        cv::warpAffine(image, rotate_result, rotationMatrix, image.size());
    for(int i = 0 ; i < runNum ; i++){  // 实测
        auto tp_start = std::chrono::system_clock::now();
        cv::warpAffine(image, rotate_result, rotationMatrix, image.size());
        auto tp_end = std::chrono::system_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(tp_end - tp_start).count();
    }
    print_performance("rotate", total_time / runNum);
    cv::imwrite("../result/rotate_" + size + ".jpg",rotate_result);
}

//畸变矫正
void correct(const cv::Mat &image, const std::string &size){
    int total_time = 0;
    cv::Mat correct_result;
    cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 762.111249, 0.000000, 994.975503, 0.000000, 763.725775, 585.178879, 0.000000, 0.000000, 1.000000);
    cv::Mat distortionCoefficients = (cv::Mat_<double>(1, 5) << 0.051019, -0.046566, 0.010455, 0.007456, 0.000000);
    for(int i = 0 ; i < preNum ; i++)  // 预热
        cv::undistort(image, correct_result, cameraMatrix, distortionCoefficients);
    for(int i = 0 ; i < runNum ; i++){  // 实测
        auto tp_start = std::chrono::system_clock::now();
        cv::undistort(image, correct_result, cameraMatrix, distortionCoefficients);
        auto tp_end = std::chrono::system_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(tp_end - tp_start).count();
    }
    print_performance("correct", total_time / runNum);
    cv::imwrite("../result/correct_" + size + ".jpg",correct_result);
}

//边缘检测
void canny(const cv::Mat &image, const std::string &size){
    int total_time = 0;
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat canny_result;
    for(int i = 0 ; i < preNum ; i++)  // 预热
        cv::Canny(gray, canny_result, 50, 150); 
    for(int i = 0 ; i < runNum ; i++){  // 实测
        auto tp_start = std::chrono::system_clock::now();
        cv::Canny(gray, canny_result, 50, 150); 
        auto tp_end = std::chrono::system_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(tp_end - tp_start).count();
    }
    print_performance("canny", total_time / runNum);
    cv::imwrite("../result/canny_" + size + ".jpg",canny_result);
}

//人脸检测
void face_detect(const cv::Mat &image, const std::string &size){
    int total_time = 0;
    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")) {
        std::cerr << "Error: Could not load face cascade." << std::endl;
        return;
    }
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat face_detect_result = image.clone();
    std::vector<cv::Rect> faces;
    for(int i = 0 ; i < preNum ; i++)  // 预热
        faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(100, 100));
    for(int i = 0 ; i < runNum ; i++){  // 实测
        auto tp_start = std::chrono::system_clock::now();
        faceCascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(100, 100));
        auto tp_end = std::chrono::system_clock::now();
        total_time += std::chrono::duration_cast<std::chrono::microseconds>(tp_end - tp_start).count();
    }
    for (const auto& face : faces) {
        cv::rectangle(face_detect_result, face, cv::Scalar(0, 255, 0), 2);
    }
    print_performance("face_detect", total_time / runNum);
    cv::imwrite("../result/face_detect_" + size + ".jpg",face_detect_result);
}

int main() {
    cv::Mat image_512 = cv::imread("../images/512w512h_raw.jpg");
    cv::Mat image_983 = cv::imread("../images/983w553h_raw.jpg");
    cv::Mat image_1280 = cv::imread("../images/1280w720h_resize.jpg");
    cv::Mat image_1920 = cv::imread("../images/1920w1080h_resize.jpg");
    cv::Mat image_2560 = cv::imread("../images/2560w1440h_resize.jpg");
    cv::Mat image_3840 = cv::imread("../images/3840w2160h_resize.jpg");
    cv::Mat image_correct = cv::imread("../images/correct_1920x1080.jpeg");

    std::vector<cv::Mat> images = {image_512, image_983, image_1280, image_1920, image_2560, image_3840};
    std::vector<std::string> sizes = {"512x512", "983x553", "1280x720", "1920x1080", "2560x1440", "3840x2160"};
    
    if (fs::exists("../result_cpp") != true) {
        fs::create_directory("../result_cpp");
    }

    // 获取当前进程pid
    pid_t pid = getpid();

    // 核心全开
    cpu_set_t cpuSet;
    CPU_ZERO(&cpuSet);
    for (int i = 0; i < std::thread::hardware_concurrency(); ++i)
        CPU_SET(i, &cpuSet);
    if (sched_setaffinity(pid, sizeof(cpu_set_t), &cpuSet) == 0) {
        std::cout << "\n\033[1;32;41m Process bound to all CPU core \033[0m" << std::endl;
    } else {
        std::cerr << "\n\033[1;32;41m Error: Unable to set CPU affinity\033[0m" << std::endl;
        return -1;
    }
    // 记录输出数据到文本文件
    FPS_DATA  << "00_cpp_test_all_core.cpp\n FPS DATA\n";
    MS_DATA << "00_cpp_test_all_core.cpp\n MS DATA\n";
    // 开始跑分
	for (int i = 0 ; i < images.size() ; i++)
	{
        std::cout<<"\n\033[32m----------------- "<<sizes[i]<<"(default) -----------------\033[0m"<<std::endl;
        guassian(images[i], sizes[i]);
        blur(images[i], sizes[i]);
        resize_LINEAR(images[i], sizes[i]);
        resize_NEAREST(images[i], sizes[i]);
        rotate(images[i], sizes[i]);
        canny(images[i], sizes[i]);
        correct(images[i], sizes[i]);
        face_detect(images[i], sizes[i]);
	}
    // 按照测试顺序保存到本地
    std::string saveData = FPS_DATA.str() + MS_DATA.str();
    std::ofstream outfile("../log_00_cpp_test_all_core.txt", std::ios::out);
    if (!outfile.is_open()) {
        std::cerr << "Unable to open file";
        return 1;
    }
    outfile << saveData;
    outfile.close();

    return 0;
}