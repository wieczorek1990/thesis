#include <iostream>
#include "detector.hpp"

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
    char* windowName = (char*) "result";
    try {
        if(argc != 3) {
            cout << "Usage: ocv6 filename type" << endl;
            return EXIT_FAILURE;
        }
        Detector d(argv[1], argv[2]);
        Mat result = d.getResult();
        cout << d.getCenters();
        cout.flush();
        namedWindow(windowName, CV_WINDOW_NORMAL);
        imshow(windowName, result);
        waitKey(0);
    } catch (string text) {
        cout << text << endl;
    }
    return EXIT_SUCCESS;
}
