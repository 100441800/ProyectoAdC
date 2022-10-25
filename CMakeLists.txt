cmake_minimum_required(VERSION 3.23)
project(image LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS_OFF)

set(SOURCES ${PROJECT_SOURCE_DIR}/common/progargs.cpp ${PROJECT_SOURCE_DIR}/common/image.cpp ${PROJECT_SOURCE_DIR}/aos/imageaos.cpp ${PROJECT_SOURCE_DIR}/image-aos/imgaos.cpp)

#add_compile_options(−Wall −Wextra −Werror −pedantic −pedantic−errors)
add_compile_options(-Wall -Wextra -pedantic -O3 -march=native)
include_directories(${PROJECT_SOURCE_DIR})
add_library(common ${PROJECT_SOURCE_DIR}/common/progargs.cpp ${PROJECT_SOURCE_DIR}/common/image.cpp)
add_library(soa ${PROJECT_SOURCE_DIR}/soa/imagesoa.cpp)
add_library(aos ${PROJECT_SOURCE_DIR}/aos/imageaos.cpp ${PROJECT_SOURCE_DIR}/common/image.cpp)

add_executable(imgsoa ${PROJECT_SOURCE_DIR}/image-soa/imgsoa.cpp)
target_link_libraries(imgsoa common soa)
add_executable(imgaos ${PROJECT_SOURCE_DIR}/image-aos/imgaos.cpp)
target_link_libraries(imgaos PRIVATE common aos)
