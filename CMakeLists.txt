cmake_minimum_required(VERSION 3.10)

project(MyProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Ceres REQUIRED)
find_package(Glog REQUIRED)
# COLMAP also usually needs Boost and FreeImage
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(COLMAP REQUIRED)
find_package(TBB REQUIRED)



add_executable(my_app
    main.cpp
    findImageCoords/ArUcoDetector.cpp
    findLength/findLength.cpp
    scalePointcloud/scalePointcloud.cpp
)


link_directories(/usr/local/lib)

target_link_libraries(my_app PRIVATE
    ${OpenCV_LIBRARIES}
    colmap_scene
    colmap_estimators
    colmap_sfm
    colmap_image
    colmap_sensor
    colmap_geometry
    colmap_util
    colmap_math
    OpenImageIO::OpenImageIO
    ${Boost_LIBRARIES}
    ceres
    glog::glog
    freeimage
    curl        # Fixes curl_easy_init
    crypto      # Fixes SHA256 (OpenSSL)
    pthread     # Fixes threading
    dl          # Fixes dynamic loading symbols
    TBB::tbb

)




target_include_directories(my_app
    PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    /usr/local/include
    ${COLMAP_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
)


target_precompile_headers(my_app PRIVATE
    <Eigen/Core>
    <Eigen/Dense>
    <opencv2/opencv.hpp>
    <ceres/ceres.h>
    <vector>
    <string>
    <iostream>
    <colmap/scene/reconstruction.h>
)


