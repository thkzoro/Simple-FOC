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
# 定义公共库
set(COMMON_LIBS simplefoc libgpiodcxx.a libgpiod.a)

# 定义一个函数来简化添加可执行文件和链接库的过程
function(add_example name source_file)
    add_executable(${name} ${source_file})
    target_link_libraries(${name} ${COMMON_LIBS})
endfunction()


add_example(simplefocmini_openloop
        examples_linux/hardware_specific_examples/SimpleFOCMini/open_loop/open_loop.cpp)

add_example(as5600_position_control
        examples_linux/hardware_specific_examples/SimpleFOCMini/position_control/as5600_position_control.cpp)

add_example(open_loop_velocity_example
        examples_linux/motion_control/open_loop_motor_control/open_loop_velocity_example/open_loop_velocity_example.cpp)

add_example(open_loop_position_example
        examples_linux/motion_control/open_loop_motor_control/open_loop_position_example/open_loop_position_example.cpp)

add_example(angle_control
        examples_linux/motion_control/position_motion_control/magnetic_sensor/angle_control/angle_control.cpp)

add_example(velocity_control
        examples_linux/motion_control/velocity_motion_control/magnetic_sensor/velocity_control/velocity_control.cpp)

#utils
add_example(find_sensor_offset_and_direction
        examples_linux/utils/calibration/find_sensor_offset_and_direction/find_sensor_offset_and_direction.cpp)

add_example(magnetic_sensor_i2c_example
        examples_linux/utils/sensor_test/magnetic_sensors/magnetic_sensor_i2c/magnetic_sensor_i2c_example/magnetic_sensor_i2c_example.cpp)
# 添加编译选项
#target_compile_options(simplefoc PRIVATE -Wall -Wextra -O2)
# 如果需要链接其他库（如实时库），可以添加：
# target_link_libraries(simplefoc rt)
