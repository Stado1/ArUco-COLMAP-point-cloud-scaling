#include "findLength.hpp"



double findAverageDistanceBetweenPoints(const Coordinates& data, const bool filter)
{

    double averageDistance = 0.0;
    double totalDistance = 0.0;
    // Access the data through the getter
    const auto& results = data.getData();

    // loop through all the markers coordinates, and all the side lengths of each marker
    for (int i=0; i<results.size(); i++) {
        // std::cout << i << ":  " << (results[i].markerCorners3d[0] - results[i].markerCorners3d[1]).norm() << std::endl;
        totalDistance += (results[i].markerCorners3d[0] - results[i].markerCorners3d[1]).norm();
        totalDistance += (results[i].markerCorners3d[1] - results[i].markerCorners3d[2]).norm();
        totalDistance += (results[i].markerCorners3d[2] - results[i].markerCorners3d[3]).norm();
        totalDistance += (results[i].markerCorners3d[3] - results[i].markerCorners3d[0]).norm();
    }
    // find average
    averageDistance = totalDistance / (results.size() * 4);
    // std::cout << "UNfiltered averageDistance = " << averageDistance << std::endl;

    if (filter) {
        averageDistance = findFilteredAverageDistanceBetweenPoints(data, averageDistance);
    }

    return averageDistance;
}


double findFilteredAverageDistanceBetweenPoints(const Coordinates& data, const double& averageOrgDistance)
{
    double percentDeviation = 0.3;
    double upperLimit = averageOrgDistance * (1.0+percentDeviation);
    double lowerLimit = averageOrgDistance * (1.0-percentDeviation);

    double averageDistance = 0.0;
    double totalDistance = 0.0;
    int members = 0;
    double len = 0.0;
    // Access the data through the getter
    const auto& results = data.getData();


    // loop through all the markers coordinates, and all the side lengths of each marker
    for (int i=0; i<results.size(); i++) {
//
        len = (results[i].markerCorners3d[0] - results[i].markerCorners3d[1]).norm();
        if (len < upperLimit) {
            if (len > lowerLimit) {
                totalDistance += len;
                members++;
            }
        }

        len = (results[i].markerCorners3d[1] - results[i].markerCorners3d[2]).norm();
        if (len < upperLimit) {
            if (len > lowerLimit) {
                totalDistance += len;
                members++;
            }
        }

        len = (results[i].markerCorners3d[2] - results[i].markerCorners3d[3]).norm();
        if (len < upperLimit) {
            if (len > lowerLimit) {
                totalDistance += len;
                members++;
            }
        }

        len = (results[i].markerCorners3d[3] - results[i].markerCorners3d[0]).norm();
        if (len < upperLimit) {
            if (len > lowerLimit) {
                totalDistance += len;
                members++;
            }
        }




    }
    // find average
    averageDistance = totalDistance / (members);
    // std::cout << " members = " << members << std::endl;
    // std::cout << "  filtered averageDistance = " << averageDistance << std::endl;


    return averageDistance;

}
