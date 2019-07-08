#include "detector.hpp"

Detector::Detector(string filename, string _type) {
    src = imread(filename);
    type = _type;
    if(!src.data) {
        throw "Couldn't load image.";
    }
    result = ellipses(morphology());
}

Mat Detector::morphology() {
    Mat result;
    if (type.compare("whole") == 0 || type.compare("half") == 0) {
        Mat src_gray;
        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        cvtColor(src, src_gray, CV_BGR2GRAY);
        Canny(src_gray, canny_output, thresh, thresh * 2);
        findContours(canny_output, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        for(size_t i = 0; i < contours.size(); i++) {
            if (hierarchy[i][3] != -1) {
                Moments m = moments(contours[i], false);
                Point2f p = Point2f(m.m10/m.m00 , m.m01/m.m00);
                floodFill(src, p, Scalar(0, 0, 0));
            }
        }
        result = src;
    }
    if (type.compare("half") == 0 || type.compare("quarter") == 0 || type.compare("eighth") == 0 || type.compare("sixteenth") == 0) {
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2*kernelSize + 1, 2*kernelSize+1), Point(kernelSize, kernelSize));
        morphologyEx(src, result, MORPH_CLOSE, element);
    }
    return result;
}

Mat Detector::ellipses(Mat src) {
    vector<vector<Point> > contours;
    Mat bimage, src_gray;
    cvtColor(src, src_gray, CV_RGB2GRAY);
    threshold(src_gray, bimage, thresh, threshMax, THRESH_BINARY_INV);
    findContours(bimage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    Mat cimage = Mat::zeros(bimage.size(), CV_8UC3);
    for(size_t i = 0; i < contours.size(); i++) {
        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);
        drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);
        Point2f vtx[4];
        box.points(vtx);
        for(int j = 0; j < 4; j++)
            line(cimage, vtx[j], vtx[(j+1)%4], Scalar(0,255,0), 1, CV_AA);
        centers.push_back(box.center);
    }
    return cimage;
}

Mat Detector::getResult() {
    return result;
}

vector<Point2f> Detector::getCenters() {
    return centers;
}
