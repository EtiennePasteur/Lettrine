#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "BlobResult.h"
#include <boost/format.hpp>

using namespace cv;
using namespace std;

template<typename... Args>
constexpr auto fmt(Args &&... args) {
    return (boost::format(std::forward<Args>(args)...));
}

typedef struct s_pos_contour {
    Point max;
    Point min;
    vector<Point> contour;
} t_pos_contour;

void removeSmallElem(Mat &img, int size) {
    CBlobResult blobs;
    blobs = CBlobResult(img, Mat(), 4);
    blobs.Filter(blobs, B_INCLUDE, CBlobGetLength(), B_GREATER, size);

    Mat newimg(img.size(), img.type());
    newimg.setTo(0);
    for (int i = 0; i < blobs.GetNumBlobs(); i++) {
        blobs.GetBlob(i)->FillBlob(newimg, CV_RGB(255, 255, 255), 0, 0, true);
    }
    img = newimg;
}

void findPics(const Mat &image, std::vector<t_pos_contour> &contoursPos) {
    vector<vector<Point> > contours;
    vector<vector<Point> > contours0;
    vector<Vec4i> h;
    findContours(image, contours0, h, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    contours.resize(contours0.size());
    for (size_t k = 0; k < contours0.size(); k++)
        approxPolyDP(Mat(contours0[k]), contours[k], 3, true);

    bool remove = false;
    t_pos_contour valueContour;
    double seuilX = image.size().width * 0.07;
    double seuilY = image.size().height * 0.07;
    for (std::vector<vector<Point>>::iterator itV = contours.begin(); itV != contours.end();) {
        valueContour.max = {0, 0};
        valueContour.min = {image.size().width, image.size().height};
        valueContour.contour = *itV;
        for (std::vector<Point>::iterator itP = itV->begin(); itP != itV->end(); itP++) {
            if (itP->x < 10 || itP->x > image.size().width - 10 || itP->y < 10 || itP->y > image.size().height - 10) {
                itV = contours.erase(itV);
                remove = true;
                break;
            }
            if (valueContour.max.x < itP->x)
                valueContour.max.x = itP->x;
            if (valueContour.max.y < itP->y)
                valueContour.max.y = itP->y;
            if (valueContour.min.x > itP->x)
                valueContour.min.x = itP->x;
            if (valueContour.min.y > itP->y)
                valueContour.min.y = itP->y;
        }
        if (!remove) {
            ++itV;
        } else {
            remove = false;
            continue;
        }
        if (valueContour.max.x - valueContour.min.x > seuilX && valueContour.max.y - valueContour.min.y > seuilY)
            contoursPos.push_back(valueContour);
    }
}

void createJpeg(const std::string &path, const std::string &destination, const std::vector<t_pos_contour> &contoursPos) {
    Mat image = imread(path, CV_LOAD_IMAGE_UNCHANGED);
    int imgNum = 1;
    for (auto &i: contoursPos) {
        cv::Rect dimensions(i.min.x, i.min.y, i.max.x, i.max.y);
        cv::Mat croppedImage = image(dimensions);
        std::string filename = (fmt(destination) % imgNum++).str();
        std::cout << filename << std::endl;
//        try {
//            imwrite(filename, croppedImage, vector<int>({CV_IMWRITE_JPEG_QUALITY, 95}));
//        }
//        catch (runtime_error &ex) {
//            fprintf(stderr, "Exception converting image to JPG format: %s\n", ex.what());
//        }
    }
}

void extractPics(const std::string &path, const std::string &destPath) {
    Mat imageP = imread(path, 1);
    std::vector<t_pos_contour> contoursPos;
    cvtColor(imageP, imageP, CV_RGB2GRAY);
    threshold(imageP, imageP, 75.0, 255.0, THRESH_BINARY_INV);
    removeSmallElem(imageP, 800);
    findPics(imageP, contoursPos);
    createJpeg(path, destPath, contoursPos);
}
