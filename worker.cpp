#include <worker.h>
#include <opencv2/core/mat.hpp>
using namespace cv;
using namespace std;

Worker::Worker() {
    qDebug() << files;
    // you could copy data from constructor arguments to internal variables here.
}

void Worker::process() {
    qDebug() << files;
    qDebug() << this->selectedRow;
    runCoregistration(0,0);
    qDebug() << "process";
}

void Worker::stopProgress() {
    emit finished();
    qDebug() << "stopProgress";
}

// --- DECONSTRUCTOR ---
Worker::~Worker() {
    qDebug() << "3333";
    // free resources
}

Mat Worker::GetGradient(Mat src_gray)
{
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    int scale = 1;
    int delta = 0;
    int ddepth = CV_32FC1; ;

    // Calculate the x and y gradients using Sobel operator

    Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    // Combine the two gradients
    Mat grad;
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

    return grad;

}


int Worker::runCoregistration( int argc, char** argv )
{

    // Read 8-bit color image.
    // This is an image in which the three channels are
    // concatenated vertically.
    Mat im_master =  imread("D:/Documenti/Didattica/telerilevamento/dataset/camera_nir/DSC_0242.JPG");
    Mat im_slave =  imread("D:/Documenti/Didattica/telerilevamento/dataset/camera_nir/DSC_0243.JPG");

    // Convert images to gray scale;
    Mat im_master_gray, im_slave_gray;
    cvtColor(im_master, im_master_gray, cv::COLOR_BGR2GRAY);
    cvtColor(im_slave, im_slave_gray, cv::COLOR_BGR2GRAY);

    // Define the motion model
    const int warp_mode = MOTION_HOMOGRAPHY;

    // Set a 2x3 or 3x3 warp matrix depending on the motion model.
    Mat warp_matrix;

    // Initialize the matrix to identity
    if ( warp_mode == MOTION_HOMOGRAPHY )
        warp_matrix = Mat::eye(3, 3, CV_32F);
    else
        warp_matrix = Mat::eye(2, 3, CV_32F);

    // Specify the number of iterations.
    int number_of_iterations = 1000;

    // Specify the threshold of the increment
    // in the correlation coefficient between two iterations
    double termination_eps = 1e-10;

    // Define termination criteria
    TermCriteria criteria (TermCriteria::COUNT+TermCriteria::EPS, number_of_iterations, termination_eps);

    // Run the ECC algorithm. The results are stored in warp_matrix.
    findTransformECC(
                     im_master_gray,
                     im_slave_gray,
                     warp_matrix,
                     warp_mode,
                     criteria
                 );

    // Storage for warped image.
    Mat im_slave_aligned;

    if (warp_mode != MOTION_HOMOGRAPHY)
        // Use warpAffine for Translation, Euclidean and Affine
        warpAffine(im_slave, im_slave_aligned, warp_matrix, im_master.size(), INTER_LINEAR + WARP_INVERSE_MAP);
    else
        // Use warpPerspective for Homography
        warpPerspective (im_slave, im_slave_aligned, warp_matrix, im_master.size(),INTER_LINEAR + WARP_INVERSE_MAP);

    emit finished();
    // Show final output
   imwrite("D:/Documenti/Didattica/telerilevamento/dataset/camera_nir/DSC_0243Aligned.JPG", im_slave_aligned);
   // waitKey(0);
    return(0);
}

