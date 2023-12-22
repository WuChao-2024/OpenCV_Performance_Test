/*
g++ cpp_cv_test.cpp -o cpp_cv_test `pkg-config --cflags --libs opencv`
*/
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    // cv::Mat image(200, 200, CV_8UC3, cv::Scalar(0, 0, 255));
    // cv::imwrite("output_image.jpg", image);
    // std::cout << "Image created." << std::endl;
    return 0;
}