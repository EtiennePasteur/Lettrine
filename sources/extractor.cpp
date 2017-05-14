// std and boost utils
#include <boost/format.hpp>
template<typename... Args>
constexpr auto fmt(Args &&... args) {
    return (boost::format(std::forward<Args>(args)...));
}
#include <iostream>

// bloblib and opencv
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "BlobResult.h"

using namespace cv;
using namespace std;

struct t_pos_contour {
    Point max;
    Point min;
    vector<Point> contour;
};

static Mat _ImageP;

static void removeSmallElem(int size) {
    CBlobResult blobs;
    blobs = CBlobResult(_ImageP, Mat(), 4);
    blobs.Filter(blobs, B_INCLUDE, CBlobGetLength(), B_GREATER, size);

    Mat newimg(_ImageP.size(), _ImageP.type());
    newimg.setTo(0);
    for (int i = 0; i < blobs.GetNumBlobs(); i++) {
        blobs.GetBlob(i)->FillBlob(newimg, CV_RGB(255, 255, 255), 0, 0, true);
    }
    _ImageP = newimg;
}

static void findPics(std::vector<t_pos_contour> &contoursPos) {
    vector<vector<Point> > contours;
    vector<vector<Point> > contours0;
    findContours(_ImageP, contours0, vector<Vec4i>{}, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    contours.resize(contours0.size());
    for (size_t k = 0; k < contours0.size(); k++)
        approxPolyDP(Mat(contours0[k]), contours[k], 3, true);

    bool remove = false;
    t_pos_contour valueContour;
    double seuilX = _ImageP.size().width * 0.07;
    double seuilY = _ImageP.size().height * 0.07;
    for (std::vector<vector<Point>>::iterator itV = contours.begin(); itV != contours.end();) {
        valueContour.max = {0, 0};
        valueContour.min = { _ImageP.size().width, _ImageP.size().height };
        valueContour.contour = *itV;
        for (std::vector<Point>::iterator itP = itV->begin(); itP != itV->end(); itP++) {
            if (itP->x < 10 || itP->x > _ImageP.size().width - 10 || itP->y < 10 || itP->y > _ImageP.size().height - 10) {
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

static void createJpeg(std::string const &path, std::string const &destination, std::vector<t_pos_contour> const &contoursPos) {
    int imgNum = 1;
    int margin = 10;
    Mat image = imread(path, CV_LOAD_IMAGE_UNCHANGED);
    for (auto &&i: contoursPos) {
        cv::Rect myROI(i.min.x - margin, i.min.y - margin, (i.max.x + 2 * margin) - i.min.x, (i.max.y + 2 * margin) - i.min.y);
        cv::Mat croppedImage = image(myROI);
        std::string filename = (fmt(destination) % imgNum++).str();
        try {
            std::cout << "Writing " << filename << std::endl;
            imwrite(filename, croppedImage, vector<int>({CV_IMWRITE_JPEG_QUALITY, 95}));
        }
        catch (runtime_error &ex) {
            fprintf(stderr, "Exception converting image to JPG format: %s\n", ex.what());
        }
    }
}

void extractPics(std::string const &path, std::string const &destPath) {
  _ImageP = imread(path, 1);
    std::vector<t_pos_contour> contoursPos;
    cvtColor(_ImageP, _ImageP, CV_RGB2GRAY);
    threshold(_ImageP, _ImageP, 75.0, 255.0, THRESH_BINARY_INV);
    removeSmallElem(800);
    findPics(contoursPos);
    createJpeg(path, destPath, contoursPos);
}
