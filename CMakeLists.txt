cmake_minimum_required(VERSION 3.10)

project(tflod VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV 4.5 REQUIRED)

set(TENSORFLOW_SOURCE_DIR "" CACHE PATH "Directory that contains the TensorFlow project")
if(NOT TENSORFLOW_SOURCE_DIR)
    get_filename_component(TENSORFLOW_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../tensorflow" ABSOLUTE)
endif()
include_directories(. ${OpenCV_INCLUDE_DIRS} ${TENSORFLOW_SOURCE_DIR})

add_executable(${PROJECT_NAME}
                src/main.cpp
                src/utils.cpp
                src/string_utils.cpp
                src/os_utils_linux.cpp
                src/file_utils.cpp
                src/config_utils.cpp
                src/video_capture_utils.cpp
                src/tflite_utils.cpp
                src/tflite_od_utils.cpp
                src/fps_utils.cpp
                src/tflite_od_glue.cpp
                src/tflite_data.cpp
                src/cv_utils.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${TENSORFLOW_SOURCE_DIR})

link_directories("${TENSORFLOW_SOURCE_DIR}/tensorflow/lite/tools/make/gen/linux_aarch64/lib" "/usr/local/lib" )

target_link_libraries(${PROJECT_NAME} dl pthread
                        ${OpenCV_LIBS}
                        ${TENSORFLOW_SOURCE_DIR}/tensorflow/lite/tools/make/gen/linux_aarch64/lib/libtensorflow-lite.a)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
