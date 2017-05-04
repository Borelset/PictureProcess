#include "mainwindow.h"
#include <QApplication>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <time.h>

using namespace cv;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

    /*
    Mat img = imread("d:/test.jpg");
    imshow("demo", img);
    waitKey(0);
    */

}
