#ifndef birdview_h
#define birdview_h

#pragma once

#include <iostream>
#include "birdview.h"
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/imgproc.hpp>
#include <thread>
#include <unistd.h>


#endif

using namespace std;
using namespace cv;

BirdView::BirdView()
{

}

void BirdView::CallBackFunc(int event, int x, int y, int flags, void *leftCoordinate)
{
    Point *p = (Point*) leftCoordinate;
    if  ( event == EVENT_LBUTTONDOWN )
    {
         cout << "Left button position (" << x << ", " << y << ")" << endl;
         p->x = x;
         p->y = y;
         cout << "this is the pointer  : " << *p << endl;
    }
}

void BirdView::SetView()
{
    int down_width = 250;
    int down_height = 480;
    Mat resize_down;

    Mat frame1;
    Mat frame2;
    Mat frame3;
    Mat frame4;
    Mat test;

    VideoCapture cap1("/home/koluman/Qt Files/opencv_2/Yeni Kayıt/Calibration_Files/Front/front_record.mp4");
    VideoCapture cap2("/home/koluman/Qt Files/opencv_2/Yeni Kayıt/Calibration_Files/Left/left_record.mp4");
    VideoCapture cap3("/home/koluman/Qt Files/opencv_2/Yeni Kayıt/Calibration_Files/Right/right_record.mp4");
    VideoCapture cap4("/home/koluman/Qt Files/opencv_2/Yeni Kayıt/Calibration_Files/Back/back_record.mp4");

    if(!cap1.isOpened()){  // check if we succeeded
        cout<<"Cap1 is not opened";}
    if(!cap2.isOpened()){  // check if we succeeded
        cout<<"Cap1 is not opened";}
    if(!cap3.isOpened()){  // check if we succeeded
        cout<<"Cap1 is not opened";}
    if(!cap4.isOpened()){  // check if we succeeded
        cout<<"Cap1 is not opened";}
    //511,397

    cv::Point2f srcPointsF[] = {Point(258,258),Point(404,263),Point(174,345),Point(560,413)};
    cv::Point2f dstPointsF[] = {Point(0,0),Point(640,0),Point(0,80),Point(640,80)};
    cv::Point2f srcPointsL[] = {Point(168,248),Point(417,254),Point(36,300),Point(534,334)};
    cv::Point2f dstPointsL[] = {Point(0,0),Point(640,0),Point(0,120),Point(640,120)};
    cv::Point2f srcPointsR[] = {Point(216,256),Point(479,258),Point(86,338),Point(634,325)};
    cv::Point2f dstPointsR[] = {Point(0,0),Point(640,0),Point(0,120),Point(640,120)};
    cv::Point2f srcPointsB[] = {Point(216,247),Point(427,240),Point(77,304),Point(608,300)};
    cv::Point2f dstPointsB[] = {Point(0,0),Point(640,0),Point(0,60),Point(640,60)};

    Mat F = getPerspectiveTransform(srcPointsF, dstPointsF);
    Mat L = getPerspectiveTransform(srcPointsL, dstPointsL);
    Mat R = getPerspectiveTransform(srcPointsR, dstPointsR);
    Mat B = getPerspectiveTransform(srcPointsB, dstPointsB);

    Mat output;

    Mat texture_1 = cv::Mat::zeros(cv::Size(640,480), CV_8U);
    cvtColor(texture_1, texture_1, COLOR_GRAY2BGR);

    cv::Mat maskR = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
    vector<Point> pts3 = {Point(640,0),Point(640,480),Point(468,400),Point(468,80)};
    fillPoly(maskR,pts3,Scalar(255));
    //maskR(Rect(420,0,220,480)) = 255;

    cv::Mat maskL = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
    vector<Point> pts2 = {Point(0,480),Point(0,0),Point(190,80),Point(190,400)};
    fillPoly(maskL,pts2,Scalar(255));
    //maskL(Rect(0,0,220,480)) = 255;

    cv::Mat maskF = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
    vector<Point> pts1 = {Point(0,0),Point(640,0),Point(468,80),Point(190,80)};
    fillPoly(maskF,pts1,Scalar(255));
    //maskF(Rect(0,0,640,90)) = 255;

    cv::Mat maskB = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
    vector<Point> pts4 = {Point(640,480),Point(0,480),Point(190,400),Point(468,400)};
    fillPoly(maskB,pts4,Scalar(255));
    //maskB(Rect(0,400,640,80)) = 255;

    cvtColor(maskR, maskR, COLOR_GRAY2BGR);
    cvtColor(maskL, maskL, COLOR_GRAY2BGR);
    cvtColor(maskF, maskF, COLOR_GRAY2BGR);
    cvtColor(maskB, maskB, COLOR_GRAY2BGR);

    cv::Mat resR;
    cv::Mat resL;
    cv::Mat resF;
    cv::Mat resB;

    if(1){

        cap1>>frame1;
        cap1>>test;
        cap2>>frame2;
        cap3>>frame3;
        cap4>>frame4;


        warpPerspective(frame1,frame1,F,Size(640,480));
        warpPerspective(frame2,frame2,L,Size(640,480));
        cv::rotate(frame2, frame2, cv::ROTATE_90_COUNTERCLOCKWISE);
        warpPerspective(frame3,frame3,R,Size(640,480));
        cv::rotate(frame3, frame3, cv::ROTATE_90_CLOCKWISE);
        warpPerspective(frame4,frame4,B,Size(640,480));
        cv::rotate(frame4, frame4, cv::ROTATE_180);

        resize(frame2, frame2, Size(640, 480), INTER_LINEAR);
        resize(frame3, frame3, Size(640, 480), INTER_LINEAR);

        bitwise_and(frame3,maskR,resR);
        bitwise_and(frame2,maskL,resL);
        bitwise_and(frame1,maskF,resF);
        bitwise_and(frame4,maskB,resB);

        //imshow("MaskedL",resR);
        //imshow("MaskedR",resL);

        texture_1=texture_1+resR+resL+resB+resF;
        resize(texture_1, resize_down, Size(down_width, down_height), INTER_LINEAR);

        imshow("Stitched",resize_down);

        //imshow("Masked",mask);
        //imshow("Front",frame1);
        //imshow("Front",maskF);
        //imshow("Left",frame2);
        //imshow("Right",frame3);
        //imshow("Back",resB);


        //cout<<"Mask depth:"<<maskL.depth()<<", Frame depth:"<<frame3.depth()<<endl<<"Mask channels:"<<maskL.channels()<<",Frame depth:"<<frame3.channels()<<endl;
        //cout<<"Test: "<<maskL.size()<<","<<frame3.size()<<endl;


    }

    Point TestP;
           setMouseCallback("Front", CallBackFunc, &test);

           cout << "The coordinates : x = " << TestP.x << " y = " << TestP.y << endl;

           setMouseCallback("Left", CallBackFunc, &frame2);

           cout << "The coordinates : x = " << TestP.x << " y = " << TestP.y << endl;

           setMouseCallback("Right", CallBackFunc, &frame3);

           cout << "The coordinates : x = " << TestP.x << " y = " << TestP.y << endl;

           setMouseCallback("Back", CallBackFunc, &frame4);

           cout << "The coordinates : x = " << TestP.x << " y = " << TestP.y << endl;

    cap1.release();
    cap2.release();
    cap3.release();
    cap4.release();

    waitKey(0);
    destroyAllWindows();
}
