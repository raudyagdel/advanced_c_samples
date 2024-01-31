cmake_minimum_required(VERSION 3.27)
project(advanced_c_samples C)

set(CMAKE_C_STANDARD 99)

# Copy the resources folder to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Provide the path to libjpeg-turbo
set(JPEG_INCLUDE_DIR "C:/msys64/mingw64/include")
set(JPEG_LIBRARY "C:/msys64/mingw64/lib/libjpeg.a")

# Add the libjpeg include directory
include_directories(${JPEG_INCLUDE_DIR})

add_executable(advanced_c_samples main.c)

add_executable(image
        image/image_format.h
        image/image.h
        image/image.c
        image/main.c)

# Link against libjpeg
target_link_libraries(image ${JPEG_LIBRARY})