#ifndef FIND_LENGTH_HPP
#define FIND_LENGTH_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <map>
#include <Eigen/Dense>

#include "../findImageCoords/ArUcoDetector.hpp"


/**
     * @brief Function to that finds the length of all the sides of each marker and then returns the average side length.
     * @param data the struct MarkerData that has all ArUco data inside of it
     */
double findAverageDistanceBetweenPoints(const Coordinates& data, const bool filter);


/**
     * @brief Function to filter out the outliers of the measurements and then calculate the new average.
     * @param data the struct MarkerData that has all ArUco data inside of it
     * @param averageOrgDistance the double that is the average with the outliers

     */
double findFilteredAverageDistanceBetweenPoints(const Coordinates& data, const double& averageOrgDistance);


#endif
