#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Detector {
    static const int kernelSize = 50;
    static const int thresh = 254;
    static const int threshMax = 255;
    Mat src;
    Mat result;
    Mat morphology();
    Mat ellipses(Mat);
    vector<Point2f> centers;
    string type;
  public:
    Detector(string filename, string _type);
    Mat getResult();
    vector<Point2f> getCenters();
};
