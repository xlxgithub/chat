cmake_minimum_required(VERSION 3.0)
# 项目名称
project(chat)

#配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 配置最终的可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#添加头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/model)
include_directories(${PROJECT_SOURCE_DIR}/include/server)
include_directories(${PROJECT_SOURCE_DIR}/include/server/db)
include_directories(${PROJECT_SOURCE_DIR}/include/server/redis)

#加载子目录
add_subdirectory(src)