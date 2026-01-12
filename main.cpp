#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <vector>
#include <filesystem>

#include <colmap/scene/reconstruction.h>

#include "findImageCoords/ArUcoDetector.hpp"
#include "findLength/findLength.hpp"
#include "scalePointcloud/scalePointcloud.hpp"

#include <algorithm>
#include <execution>
#include <mutex>

// COMPILE
// within main dir:   cmake --build build/
//                      cmake --build build/ -- -j$(nproc)
// within build dir:   cmake --build .



using namespace colmap;



int main() {


    // type of marker is DICT_4X4_50

    double sizeOfMarker = 0.099; // length of the sides of the marker in cm

    std::string reconstructionPath = "sparse/0";
    std::string imagePath = "images/*";


    // Access the data through the getter
    Coordinates aap; // (numOfMarkers, numOfImagesOfEachMarker);
    aap.getImageCoords(imagePath);
    aap.get3dCoords(reconstructionPath);
    const auto& results = aap.getData();
    std::cout << "len of results =  " << results.size() << std::endl;


    double avgDist = findAverageDistanceBetweenPoints(aap, true);
    double scaleFactor = sizeOfMarker / avgDist;
    std::cout << "avergae distance =  " << avgDist << std::endl;
    std::cout << "scale factor (put pointcloud in cm) = " << scaleFactor << std::endl;    scaleModel(scaleFactor, reconstructionPath);




    return 0;
}
