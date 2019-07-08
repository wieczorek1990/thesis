#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QPoint>
#include <QVector>
#include <QString>

using namespace cv;

class Detector {
    static const int kernelSize = 5;
    static const int thresh = 254;
    static const int threshMax = 255;
    Mat src;
    Mat morph;
    Mat result;
    Mat morphology();
    Mat ellipses(Mat);
    QVector<QPoint> centers;
    QString type;
  public:
    Detector(QString, QString);
    Mat getResult();
    Mat getMorph();
    Mat getSrc();
    QVector<QPoint> getCenters();
    static void show(QString, Mat, int, int);
    static Mat loadImage(QString);
    static QVector<Mat> parts(QString);
    static bool lessThan(Range &, Range &);
};

#endif // DETECTOR_H
