// g++ -std=c++11 -o cpp01_GaussianBlur cpp01_GaussianBlur.cpp `pkg-config --cflags --libs opencv`
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <filesystem>

int main() {
    std::string mother_path = "img";  // 测试图片放在"./img/"路径下
    for (const auto & entry : std::filesystem::directory_iterator(mother_path)) {
        if (entry.path().extension() == ".jpg") {
            cv::Mat img = cv::imread(entry.path().string());
            int kernel_size = 9;
            int RUN_NUM = 30;
            double time_total = 0.0;
            for (int i = 0; i < RUN_NUM; ++i) {
                auto time_begin = std::chrono::high_resolution_clock::now();
                cv::GaussianBlur(img, img, cv::Size(kernel_size, kernel_size), 0);
                auto time_end = std::chrono::high_resolution_clock::now();
                time_total += std::chrono::duration_cast<std::chrono::microseconds>(time_end - time_begin).count();
            }
            std::cout << std::left << std::setw(22) << entry.path().filename().string() << ",\033[31m" << std::right << std::fixed << std::setprecision(2) << (time_total / 1000.0 / RUN_NUM) << " ms\033[0m" << std::endl;
        }
    }
    return 0;
}