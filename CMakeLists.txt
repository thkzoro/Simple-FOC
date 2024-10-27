cmake_minimum_required(VERSION 3.10)
# 设置项目名称和语言
project(SimpleFOC_Linux VERSION 1.0 LANGUAGES CXX)
# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 添加包含目录
include_directories(thirdparty/libgpiod-2.2/include)
include_directories(
        src
        src/hal
)

# 收集所有源文件
file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)
# 创建库
add_library(simplefoc STATIC ${SOURCES})

#添加链接库目录
link_directories(
        thirdparty/libgpiod-2.2/lib
        cmake-build-debug-aarch64
)
# 添加示例可执行文件
add_executable(simplefocmini_openloop
        examples_linux/hardware_specific_examples/SimpleFOCMini/open_loop/open_loop.cpp)

target_link_libraries(simplefocmini_openloop
        simplefoc
        libgpiodcxx.a
        libgpiod.a
)

# 添加编译选项
#target_compile_options(simplefoc PRIVATE -Wall -Wextra -O2)
# 如果需要链接其他库（如实时库），可以添加：
# target_link_libraries(simplefoc rt)
