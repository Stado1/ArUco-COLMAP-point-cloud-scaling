# ArUco-COLMAP-point-cloud-scaling

## Project overview
The goal of this project is to accurately scale a COLMAP pointcloud using DICT_4X4_50 ArUco markers. The scene that is photographed should have one or multiple markers with the same size in it, with a known size. These markers will be used to scale the point cloud.

### How to run:
libraries/frameworks:
- it was made on Ubuntu 24.04
- COLMAP, it needs to be compiled from scratch to access the COLMAP libraries
- OpenCV 4.6.0


1. In main.cpp change the variable "sizeOfMarker" to the length of the sides of the marker in meters.
2. Compile the code using the CMakeLists.txt.
3. After having run a COLMAP reconstruction, copy the "sparse" folder into the project folder.
4. Add the images that were used for the reconstrution to the folder: "images", the images have to be .JPG, .jpg or .png. Make sure only images are in this folder.
5. Run the C++ code by: "./build/my_app"
6. The scaled pointcloud is now stored in the "model_scaled" folder.

Here is an example video of how to run the code: https://youtu.be/llyxr1zHE1o 
In this video the COLMAP point cloud is already made, just the scaling with the code is shown.




## Scene description and data collection
This project was made with markers of size 0.099x0.099 meter. It can also work with larger and smaller markers. In order for the COLMAP to make a good point cloud there needs to be a lot of details in the environment.
Make sure that the markers are clearly visible in multiple images. Not every marker has to be in every image, but make sure that each individual marker is clearly visible in at least 3 images.
Also make sure to add some closeup images of each individual marker. The point cloud of the ArUco markers needs to be very detailed in order for the scalefactor to be accurate.




## How the code works
So once the COLMAP pointcloud has been made the following will be done:

The code loops through all the images and looks for the ArUco markers. It stores for each marker the data in a struct. The data that is stored is: the id of the marker, the name of the image file and the coordinates of the corners of the marker.
Parallel processing is used to speed up this process.

The code will then loop through the data in the struct to find the 3d coordinates for each corner for each detected marker in each image. So each marker will be assigned multiple sets of 3d coordinates, using each image that was used to detect it.
It does this by putting the 3d pointcloud points onto the image and looking which of these points is the closest to each corner of each marker. The 3d point that is closest is then stored in the struct.

Then for each marker in each image, the length of all the sides of each marker is calculated using the 3d coordinates that are projected onto that image. The average side length will be calculated using the closest 3d points. This is the average side length of the unscaled pointcloud.
In this average some outliers are taken into account, which make the scale factor less accurate. To deal with this, first the average with the outliers is calculated then the outliers are filtered out by removing every measurement that deviates more than 30% from the average. Then the average is calculated without the outliers.

Since the real side length is known the scale factor can be calculated. This scale factor is used scale the pointcloud, which is stored in a folder named: "model_scaled".

<img src="figures/codePipeline.png" alt="Logo" width="500">







## Results
To measure how accurate the pointcloud is, 3 setups will be used. In each setup there will be 2 points of which the exact distance is known. The accuracy is defined using an error margin. The relative error in each setup will be calculated as: ( measured_distance / actual_distance ) / actual_distance.
The objects used to measure the distance of, are points on pieces of paper. These points have a circumference of about 1 to 2 mm. This is big enough to affect the error margin of a small distance, so the points will be placed pretty far away from each other (about 1 meter). But because the points are very small because of that. I added a red arrow in the images pointing at the measurement points. 
I will use the COLMAP gui to find the ID of the points I am measuring and then use the python code: "distancePointsByID.py" to find the scaled distance.

### Setup 1
A desk with 2 markers. Everything is laid flat on the desk.

<img src="figures/setupDesk.png" alt="Logo" width="500">


### Setup 2
A kitchen with 2 measurement points and an ArUco on the floor and 2 ArUco markers on the wall.

<img src="figures/setupKitchen.png" alt="Logo" width="500">


### Setup 3
A garage where the measurement points are placed on a wall and 3 ArUco markers are placed on the floor.

<img src="figures/setupGarage.png" alt="Logo" width="500">



The following table has the measurement accuracy for the three different environments.

| Setup | Actual Distance | Measured Distance | Relative Error |
| :--- |  :---: | :---: | :---: |
| **Setup 1** | 0.9 m | 0.9148 m | 1.64% |
| **Setup 2** | 1.5 m | 1.5424 m | 2.83% |
| **Setup 3** | 0.9 m | 0.9164 m | 1.82% |

From this it can be seen that the average relative error is: **2.10%**.



## Future improvement
So one of the reasons why the scaling factor might be a bit off is because COLMAP does not put any 3d points exactly on the corner of each marker. The points around the corners are taken and averaged but this does not lead to the exact corner coordinates. Therefore the side lengths are not exact and thus the scalefactor is off.
One way this can be improved is with Ray-Plane intersection. The 3d points around the corner are used to create a plane and then by looking at the pixel in the image at the marker corner an exact coordinate can be found. This can be used to create more accurate side lengths and thus a more accurate scale factor.


