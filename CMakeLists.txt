cmake_minimum_required (VERSION 2.6)

project (test-ltrapi)
set(LTR_CROSS_SDK_BUILD_PATH "$ENV{HOME}/install/ltr_cross_sdk/build")
message(${LTR_CROSS_SDK_BUILD_PATH})

set(LTR_HEADERS_PATH "${LTR_CROSS_SDK_BUILD_PATH}/ltrapi/include/ltr/include/")
set(LTR_LIB_PATH "${LTR_CROSS_SDK_BUILD_PATH}/ltrapi/ltrapi/")
include_directories(${LTR_HEADERS_PATH})
link_directories(${LTR_LIB_PATH})

add_executable(test-ltrapi main.cpp CrateEu2.cpp 
               CrateReader.cpp StdoutRecorder.cpp 
               StdoutChannelRecorder.cpp functions.cpp)
target_link_libraries(test-ltrapi ltr24api)
