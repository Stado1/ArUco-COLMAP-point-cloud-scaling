#ifndef SCALE_POINTCLOUD_HPP
#define SCALE_POINTCLOUD_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <map>
#include <Eigen/Dense>
#include <colmap/scene/reconstruction.h>
#include <Eigen/Core>
#include <Eigen/Geometry>


/**
     * @brief A funciton to scale the pointcloud
     * @param scaleFactor This is a double that is used aas the scalefactor
     * @param inputPath the path to where the .bin files are stored
     */
void scaleModel(const double& scaleFactor, const std::string& inputPath);


#endif
