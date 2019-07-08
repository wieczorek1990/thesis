#include "mainwindow.h"
#include <QApplication>

//#include "detector.h"

int main(int argc, char *argv[]) {
//    if(argc != 3) {
//        qDebug() << "Usage: filename type";
//        return EXIT_FAILURE;
//    }
//    Detector d(argv[1], argv[2]);
//    qDebug() << d.getCenters();
//    Detector::show("src", d.getSrc(), 0, 0);
//    Detector::show("morph", d.getMorph(), 400, 0);
//    Detector::show("result", d.getResult(), 800, 0);
//    waitKey(0);
//    return EXIT_SUCCESS;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
