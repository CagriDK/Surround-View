#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/features2d.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/stitching.hpp>
#include <opencv4/opencv2/xfeatures2d.hpp>
#include <opencv4/opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>
#include <unistd.h>

using namespace std;
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
