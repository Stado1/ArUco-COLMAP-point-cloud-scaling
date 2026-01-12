#include "ArUcoDetector.hpp"
#include <colmap/scene/reconstruction.h>
// #include <filesystem>


// Constructor
// Coordinates::Coordinates(int amountMarkersSet, int imagesOfEachMarkerSet)
    // : amountMarkers(amountMarkersSet), imagesOfEachMarker(imagesOfEachMarkerSet) {

// }


void Coordinates::getImageCoords(const std::string& imagePath) {
    // Collect all image files
    std::vector<cv::String> imageFiles;
    cv::glob(imagePath, imageFiles, false);

    if (imageFiles.empty()) {
        std::cout << "No images found in path: " << imagePath << std::endl;
        return;
    }

    // Setup ArUco
    cv::Ptr<cv::aruco::Dictionary> dictionary =
        cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
    cv::Ptr<cv::aruco::DetectorParameters> parameters =
        cv::aruco::DetectorParameters::create();

    // Mutex for thread-safe access to shared resources (vector and cout)
    std::mutex dataMutex;

    // Parallel Loop
    std::for_each(std::execution::par, imageFiles.begin(), imageFiles.end(), [&](const cv::String& filename) {

        // Load Image (Independent)
        cv::Mat image = cv::imread(filename);
        if (image.empty()) {
            std::lock_guard<std::mutex> lock(dataMutex);
            std::cerr << "Could not read image: " << filename << std::endl;
            return; // Equivalent to 'continue' in a normal loop
        }

        // Heavy Computation (Parallel)
        // These variables are local to each thread, so no lock needed here
        std::vector<int> markerIds;
        std::vector<std::vector<cv::Point2f>> markerCorners;

        cv::aruco::detectMarkers(
            image,
            dictionary,
            markerCorners,
            markerIds,
            parameters
        );

        // Store Results (Locked/Synchronized)
        // We only lock when writing to the shared 'dataFromMarkers' vector
        {
            std::lock_guard<std::mutex> lock(dataMutex);

            // Thread-safe printing
            std::cout << "Processing: " << std::filesystem::path(filename).filename().string()
                      << " (Found " << markerIds.size() << " markers)" << std::endl;

            for(size_t i = 0; i < markerIds.size(); i++) {
                MarkerData newData;
                newData.imageFileName = std::filesystem::path(filename).filename().string();
                newData.markerId = markerIds[i];
                newData.markerCorners2d = markerCorners[i];

                // Writing to the class member vector
                dataFromMarkers.push_back(newData);
            }
        }
    });
}

void Coordinates::get3dCoords(const std::string& sparse_path) {
    // load the colmap bin files with all the data
    colmap::Reconstruction reconstruction;
    reconstruction.ReadBinary(sparse_path);

    // Define your threshold (e.g., 5 pixels)
    // const double max_pixel_dist = 15.0;
    // const double max_dist_sq = max_pixel_dist * max_pixel_dist;

    // looptrough all the detections
    for (int i=0; i < dataFromMarkers.size(); i++)
    {
        // find the image in the .bin file
        colmap::image_t image_id = colmap::kInvalidImageId;

        for (const auto& [id, recImage] : reconstruction.Images()) {
            if (recImage.Name() == dataFromMarkers[i].imageFileName) {
                image_id = id;
                break;
            }
        }
        // stop if image can't be loaded
        if (image_id == colmap::kInvalidImageId) {
            std::cerr << "Image not found\n";
            continue;
        }


        // find the closest key point to the image coordinates
        const auto& image = reconstruction.Image(image_id);
        const auto& points2D = image.Points2D();
        // loop trough all 4 coordinates
        for (int j=0; j<4; j++)
        {
            double query_x = dataFromMarkers[i].markerCorners2d[j].x;
            double query_y = dataFromMarkers[i].markerCorners2d[j].y;

            double best_dist = std::numeric_limits<double>::max();
            colmap::point3D_t best_point3D_id = colmap::kInvalidPoint3DId;
            const colmap::Image& image = reconstruction.Image(image_id);

            // Search through 2D features in the image that have a corresponding 3D point
            for (const auto& p2d : image.Points2D()) {

                if (!p2d.HasPoint3D())
                    continue;

                // checkdistance and store the point that is closest
                double dx = p2d.xy(0) - query_x;
                double dy = p2d.xy(1) - query_y;
                double dist = dx * dx + dy * dy;

                if (dist < best_dist) {
                    best_dist = dist;
                    best_point3D_id = p2d.point3D_id;
                }
            }

            if (best_point3D_id == colmap::kInvalidPoint3DId) {
                std::cerr << "Image has no visible 3D points\n";
                continue;
            }

            // // 2. Check if the best match is within the allowed radius
            // if (best_point3D_id == colmap::kInvalidPoint3DId || best_dist > max_dist_sq) {
            //     std::cerr << "No 3D point found within " << max_pixel_dist << " pixels for marker " << i << " corner " << j << "\n";
            //
            //     // Optional: Push a dummy value or handle the missing coordinate
            //     // dataFromMarkers[i].markerCorners3d.push_back(Eigen::Vector3d::Zero());
            //     continue;
            // }

            // std::cout << "id = " << best_point3D_id << std::endl;
            const auto& point3D = reconstruction.Point3D(best_point3D_id);

            dataFromMarkers[i].markerCorners3d.push_back(point3D.xyz);

        }
    }


}



// void Coordinates::removeIncompleteData() {
//     // This removes any MarkerData entry where we failed to find 3D coordinates
//     // for all 4 corners of the ArUco marker.
//     dataFromMarkers.erase(
//         std::remove_if(dataFromMarkers.begin(), dataFromMarkers.end(),
//             [](const MarkerData& m) {
//                 // Return true if the marker should be REMOVED
//                 return m.markerCorners3d.size() < 4;
//             }),
//         dataFromMarkers.end()
//     );
// }


const std::vector<MarkerData>& Coordinates::getData() const
{
    return dataFromMarkers;
}
