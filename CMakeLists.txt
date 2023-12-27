set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.12)
project(test)

# 设置OpenCV包
find_package(OpenCV REQUIRED)

# 添加可执行文件
add_executable(00_cpp_test_all_core 00_cpp_test_all_core.cpp)
# 链接OpenCV库
target_link_libraries(00_cpp_test_all_core ${OpenCV_LIBS})

# 添加可执行文件
add_executable(01_cpp_test_single_core 01_cpp_test_single_core.cpp)
# 链接OpenCV库
target_link_libraries(01_cpp_test_single_core ${OpenCV_LIBS})
