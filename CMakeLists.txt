cmake_minimum_required(VERSION 3.10)
# 设置项目名称和语言
project(SimpleFOC_Linux VERSION 1.0 LANGUAGES CXX)
# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 添加包含目录
include_directories(thirdparty/winirgPi/include)
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
# 设置输出目录
set_target_properties(simplefoc PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
# 添加示例可执行文件（如果有的话）
#add_executable(simplefoc_example examples/simple_example.cpp)
#target_link_libraries(simplefoc_example simplefoc)
# 安装规则
install(TARGETS simplefoc
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/
        DESTINATION include/simplefoc
        FILES_MATCHING PATTERN "*.h"
)
# 添加编译选项
#target_compile_options(simplefoc PRIVATE -Wall -Wextra -O2)
# 如果需要链接其他库（如实时库），可以添加：
# target_link_libraries(simplefoc rt)
# 如果需要特定的编译定义，可以添加：
# target_compile_definitions(simplefoc PRIVATE MY_DEFINE=1)