// Including boost format to easily format string with fmt function
#include <boost/format.hpp>
#include "Extractor.hpp"

template<typename... Args>
constexpr auto fmt(Args &&... args) {
    return (boost::format(std::forward<Args>(args)...));
}


//Function that remove small elements like text or dust.
static void removeSmallElem(int size, cv::Mat &image) {
    CBlobResult blobs{CBlobResult(image, cv::Mat(), 4)};
    blobs.Filter(blobs, B_INCLUDE, CBlobGetLength(), B_GREATER, size);

    cv::Mat newimg(image.size(), image.type());
    newimg.setTo(0);
    for (int i = 0; i < blobs.GetNumBlobs(); i++) {
        blobs.GetBlob(i)->FillBlob(newimg, CV_RGB(255, 255, 255), 0, 0, true);
    }
    image = newimg;
}

static void findPics(std::vector<t_pos_contour> &contoursPos, cv::Mat &image) {
    std::vector<std::vector<cv::Point>> contours;
    findContours(image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    contours.resize(contours.size());
    for (size_t k = 0; k < contours.size(); k++)
        approxPolyDP(cv::Mat(contours[k]), contours[k], 3, true);

    bool remove = false;
    t_pos_contour valueContour;
    double seuilX = image.size().width * 0.07;
    double seuilY = image.size().height * 0.07;

    //Iterate through all elem found
    for (std::vector<std::vector<cv::Point>>::iterator itV = contours.begin(); itV != contours.end();) {
        valueContour.max = {0, 0};
        valueContour.min = {image.size().width, image.size().height};

        //Iterate through each points of the contour
        for (std::vector<cv::Point>::iterator itP = itV->begin(); itP != itV->end(); itP++) {
            //Remove elem if it's contours is to close from border
            if (itP->x < 10 || itP->x > image.size().width - 10 || itP->y < 10 || itP->y > image.size().height - 10) {
                itV = contours.erase(itV);
                remove = true;
                break;
            }
            valueContour.max.x = std::max(valueContour.max.x, itP->x);
            valueContour.max.y = std::max(valueContour.max.y, itP->y);
            valueContour.min.x = std::min(valueContour.min.x, itP->x);
            valueContour.min.y = std::min(valueContour.min.y, itP->y);
        }
        if (!remove) {
            ++itV;
        } else {
            remove = false;
            continue;
        }
        //Check if elem is not to small
        if (valueContour.max.x - valueContour.min.x > seuilX && valueContour.max.y - valueContour.min.y > seuilY)
            contoursPos.push_back(valueContour);
    }
}

//Create Jpeg by croping original image and save them onto disk.
static void createJpeg(std::string const &path, std::string const &destination, std::vector<t_pos_contour> const &contoursPos) {
    int imgNum = 1;
    //TODO: Attention risque d'ASSERT si jamais le margin est trop grand ou qu'une image est au bord du cadre
    int margin = 10;
    cv::Mat image = cv::imread(path, CV_LOAD_IMAGE_UNCHANGED);
    for (auto &&i: contoursPos) {
        cv::Rect myROI(i.min.x - margin, i.min.y - margin, (i.max.x + 2 * margin) - i.min.x, (i.max.y + 2 * margin) - i.min.y);
        cv::Mat croppedImage = image(myROI);
        std::string filename = (fmt(destination) % imgNum++).str();
        try {
            std::cout << "Writing " << filename << std::endl;
            imwrite(filename, croppedImage, std::vector<int>({CV_IMWRITE_JPEG_QUALITY, 95}));
        }
        catch (std::runtime_error &ex) {
            fprintf(stderr, "Exception converting image to JPG format: %s\n", ex.what());
        }
    }
}

//Load image turn it into gray mode, apply threshold on it and then remove small elements like text or dust.
static void prepareImage(cv::Mat &image) {
    threshold(image, image, 75.0, 255.0, cv::THRESH_BINARY_INV);
    removeSmallElem(800, image);
}

void extractPics(std::string const &path, std::string const &destPath) {
    cv::Mat image = cv::imread(path, cv::IMREAD_GRAYSCALE);
    std::vector<t_pos_contour> contoursPos;
    prepareImage(image);
    findPics(contoursPos, image);
    createJpeg(path, destPath, contoursPos);
}
