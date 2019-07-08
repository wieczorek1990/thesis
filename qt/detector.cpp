#include "detector.h"

Detector::Detector(QString filename, QString type) {
    src = loadImage(filename);
    this->type = type;
    morph = morphology();
    result = ellipses(morph);
}

Mat Detector::loadImage(QString filename) {
    Mat result = imread(filename.toStdString());
    if (!result.data) {
        throw "Couldn't load image.";
    }
    return result;
}

Mat Detector::morphology() {
    Mat result = src.clone();
    if (type == "whole" || type == "half") {
        Mat src_gray;
        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        cvtColor(src, src_gray, CV_BGR2GRAY);
        Canny(src_gray, canny_output, thresh, thresh * 2);
        findContours(canny_output, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
        for (size_t i = 0; i < contours.size(); i++) {
            if (hierarchy[i][3] != -1) {
                Moments m = moments(contours[i], false);
                Point2f p = Point2f(m.m10/m.m00 , m.m01/m.m00);
                floodFill(result, p, Scalar(0, 0, 0));
            }
        }
    }
    if (type == "half" || type == "quarter" || type == "eighth" || type == "sixteenth" || type == "thirty-second") {
        Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2*kernelSize + 1, 2*kernelSize+1), Point(kernelSize, kernelSize));
        morphologyEx(result, result, MORPH_CLOSE, element);
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
    for (size_t i = 0; i < contours.size(); i++) {
        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);
        drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);
        Point2f vtx[4];
        box.points(vtx);
        for (int j = 0; j < 4; j++) {
            line(cimage, vtx[j], vtx[(j+1)%4], Scalar(0,255,0), 1, CV_AA);
        }
        Point2f p = box.center;
        centers.push_back(QPoint(qRound(p.x), qRound(p.y)));
    }
    return cimage;
}

bool Detector::lessThan(Range &r1, Range &r2) {
    return r1.start < r2.start;
}

QVector<Mat> Detector::parts(QString filename) {
    QVector<Mat> result;
    QVector<Range> ranges;
    Mat src = loadImage(filename);
    Mat src_gray;
    cvtColor(src, src_gray, CV_BGR2GRAY);
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    threshold(src_gray, threshold_output, thresh, threshMax, THRESH_BINARY_INV);
    findContours(threshold_output, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    vector<vector<Point> > contours_poly(contours.size());
    for (size_t i = 0; i < contours.size(); i++) {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
        Rect boundRect = boundingRect(Mat(contours_poly[i]));
        if (hierarchy[i][3] == -1) {
            ranges << Range(boundRect.x, boundRect.x + boundRect.width);
        }
    }
    qSort(ranges.begin(), ranges.end(), lessThan);
    foreach (Range r, ranges) {
        result << src(Range(0, src.rows), r);
    }
    return result;
}

Mat Detector::getResult() {
    return result;
}

Mat Detector::getMorph() {
    return morph;
}

Mat Detector::getSrc() {
    return src;
}

QVector<QPoint> Detector::getCenters() {
    return centers;
}

void Detector::show(QString name, Mat mat, int x, int y) {
    namedWindow(name.toStdString(), CV_WINDOW_NORMAL);
    imshow(name.toStdString(), mat);
    moveWindow(name.toStdString(), x, y);
}
