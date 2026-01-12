#ifndef ARUCO_DETECTOR_HPP
#define ARUCO_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <vector>
#include <string>
#include <map>
#include <filesystem>

#include <algorithm>
#include <execution>
#include <mutex>

#include <iostream>
#include <filesystem>

#include <colmap/scene/reconstruction.h>

/**
 * @brief Stores information about all detected ArUco marker and its 2d and 3d cooridnates
 */
struct MarkerData
{
    std::string imageFileName; ///< Name of the file where the marker was found
    int markerId;              ///< ArUco id of the marker
    std::vector<cv::Point2f> markerCorners2d; ///< 2d pixel coordinates of the 4 corners
    std::vector<Eigen::Vector3d> markerCorners3d; ///< Corresponding 3D coordinates from reconstruction
};


class Coordinates {
private:

    // int amountMarkers; ///< This is how many markers are placed in the environment
    // int imagesOfEachMarker; ///< This is how many images of each marker will be used
    std::vector<MarkerData> dataFromMarkers;


public:
    /**
     * @brief Constructor for the Coordinate finder.
     * @param amountMarkersSet Number of unique markers expected in the environment.
     * @param imagesOfEachMarkerSet Max number of detections to store per marker ID.
     */
    // explicit Coordinates(int amountMarkersSet, int imagesOfEachMarkerSet);

    /**
     * @brief Iterates through images in a directory to detect ArUco markers.
     * Stores detection data until the specified limits are reached.
     * @param imagePath Path to the directory containing image files.
     */
    void getImageCoords(const std::string& imagePath);

    /**
     * @brief Matches detected 2D marker corners to their nearest 3D points in a COLMAP reconstruction.
     * @param sparse_path Path to the COLMAP binary reconstruction file (.bin).
     */
    void get3dCoords(const std::string& sparse_path);

    /**
     * @brief Delete individual detections
     * @return const reference to vector of MarkerData.
     */
    // void removeIncompleteData();

    /**
     * @brief Access collected marker observations.
     * @return const reference to vector of MarkerData.
     */
    const std::vector<MarkerData>& getData() const;

};

#endif
