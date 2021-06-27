#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include <QFileInfo>
/**
 * OpenCV Image Alignment  Example
 *
 * Copyright 2015 by Satya Mallick <spmallick@learnopencv.com>
 *
 */

#include <QObject>
#include <QFileInfo>
#include <QDebug>
#include <QDesktopServices>

#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;

class Worker : public QObject
{
    Q_OBJECT
    void go();


signals:
    void resultReady();
    void valueChanged(int newValue);
    void finished();

public slots:
    void process();
    void stopProgress();

public:
    Worker();
    ~Worker();
    QFileInfoList files;
    int selectedRow;
    Mat GetGradient(Mat src_gray);
    int runCoregistration( int argc, char** argv );
};


#endif // WORKER_H
