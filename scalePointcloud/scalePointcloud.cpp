#include "scalePointcloud.hpp"



void scaleModel(const double& scaleFactor, const std::string& inputPath)
{

    colmap::Reconstruction reconstruction;
    reconstruction.ReadBinary(inputPath);

    for (const auto& point3D_pair : reconstruction.Points3D()) {
        reconstruction.Point3D(point3D_pair.first).xyz *= scaleFactor;
    }

    // After scaling the 3D points...
    for (auto& image_pair : reconstruction.Images()) {
        auto& image = reconstruction.Image(image_pair.first);

        // Scale the translation component
        image.CamFromWorld().translation *= scaleFactor;
    }

    reconstruction.WriteBinary("model_scaled");


}
