#include "mainwindow.h"
#include <QApplication>

using namespace std;
using namespace cv;

void CallBackFunc(int event, int x, int y, int flags, void* leftCoordinate){
    Point *p = (Point*) leftCoordinate;
    if  ( event == EVENT_LBUTTONDOWN )
    {
        cout << "Left button position (" << x << ", " << y << ")" << endl;
        p->x = x;
        p->y = y;
//        cout << "this is the pointer  : " << *p << endl;
    }
}


string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

cv::Mat fileRead(string path,cv::Mat sourceImage)
{

    cv::Mat src=sourceImage;

    FileStorage fs;
    fs.open(path,FileStorage::READ);

    if (!fs.isOpened())
    {
        cerr<<"Failed to open :"<< "ost.yaml" <<endl;
    }

    else if(fs.isOpened())
    {
        //cout<<"Fs opened correctly"<<endl;
    }

    Mat cameraMatrix;
    Mat distortionCoefficient;
    vector<Mat> rectificationMatrix;
    Mat newCameraMatrix;
    cv::Mat mapX;
    cv::Mat mapY;
    cv::Size frameSize(640,480);

    FileNode K = fs["camera_matrix"];                         // Read string sequence - Get node
    //cout<<K.mat()<<endl;
    cv::Mat Ktest=K.mat();

    FileNode k= fs["distortion_coefficients"];
    //cout<<k.mat()<<endl;
    cv::Mat ktest=k.mat();

    //Rectification Matrix (R)
    float rectification[9]={1,0,0,0,1,0,0,0,1};
    cv::Mat R(3,3,CV_32FC1,rectification);
    //cout<<R<<endl;

    cv::fisheye::estimateNewCameraMatrixForUndistortRectify(K.mat(),k.mat(),frameSize,R,newCameraMatrix,1);
    cv::fisheye::initUndistortRectifyMap(K.mat(),k.mat(),R,newCameraMatrix,frameSize,CV_32FC1,mapX,mapY);

    cv::Mat imgUndistorted;
    cv::remap(src,imgUndistorted,mapX,mapY,cv::INTER_LINEAR);

    return imgUndistorted;
}

string getTimeStr(){

        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
// Eğer soru işaretini dışarıda bırakmak istiyor isen s() boyutunu arttır ve %T time dan sonra boşluk bırakarak en sondaki soru işaretini uzağa taşı
        std::string s(34, '\0');
        std::strftime(&s[0], s.size(), "Date: %d-%m-%Y Time: %T  ", std::localtime(&now));

        return s;

}


void BirdView()
{

    Mat frontview;
    Mat backview;

    int down_width = 250;
    int down_height = 480;
    Mat resize_down;

    Mat frame1;
    Mat frame2;
    Mat frame3;
    Mat frame4;
    Mat test;

    cv::Mat imgUndistortedBack;
    cv::Mat imgUndistortedFront;
    cv::Mat imgUndistortedLeft;
    cv::Mat imgUndistortedRight;

    cv::Mat frame1original;
    cv::Mat frame2original;
    cv::Mat frame3original;
    cv::Mat frame4original;

    //    VideoCapture cap1(0);
    //    VideoCapture cap2(2);
    //    VideoCapture cap3(3);
    //    VideoCapture cap4(1);

    VideoCapture cap1("/home/cdukunlu/Qt_Files/opencv_2 1/Calibration_Files/Front/front.mp4");
    VideoCapture cap2("/home/cdukunlu/Qt_Files/opencv_2 1/Calibration_Files/Left/left.mp4");
    VideoCapture cap3("/home/cdukunlu/Qt_Files/opencv_2 1/Calibration_Files/Right/right.mp4");
    VideoCapture cap4("/home/cdukunlu/Qt_Files/opencv_2 1/Calibration_Files/Back/back.mp4");

    if(!cap1.isOpened()){  // check if we succeeded
        cout<<"Cap1 is not opened";}
    if(!cap2.isOpened()){  // check if we succeeded
        cout<<"Cap1 is not opened";}
    if(!cap3.isOpened()){  // check if we succeeded
        cout<<"Cap1 is not opened";}
    if(!cap4.isOpened()){  // check if we succeeded
        cout<<"Cap1 is not opened";}
    //511,397

    cv::Point2f srcPointsF[] = {Point(245,282),Point(394,276),Point(92,442),Point(544,414)};
    cv::Point2f dstPointsF[] = {Point(30,0),Point(610,0),Point(30,80),Point(610,80)};
    cv::Point2f srcPointsL[] = {Point(166,248),Point(430,257),Point(65,290),Point(535,327)};
    cv::Point2f dstPointsL[] = {Point(0,20),Point(640,20),Point(0,140),Point(640,140)};
    cv::Point2f srcPointsR[] = {Point(217,273),Point(488,227),Point(106,362),Point(604,252)};
    cv::Point2f dstPointsR[] = {Point(30,20),Point(630,20),Point(20,130),Point(630,130)};
    cv::Point2f srcPointsB[] = {Point(224,234),Point(420,241),Point(0,324),Point(640,350)};
    cv::Point2f dstPointsB[] = {Point(0,0),Point(640,0),Point(0,80),Point(640,80)};

    Mat F = getPerspectiveTransform(srcPointsF, dstPointsF);
    Mat L = getPerspectiveTransform(srcPointsL, dstPointsL);
    Mat R = getPerspectiveTransform(srcPointsR, dstPointsR);
    Mat B = getPerspectiveTransform(srcPointsB, dstPointsB);

    Mat output;

    Mat texture_1 = cv::Mat::zeros(cv::Size(640,480), CV_8U);
    cvtColor(texture_1, texture_1, COLOR_GRAY2BGR);


    cv::Mat maskL = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
    vector<Point> pts2 = {Point(0,480),Point(0,0),Point(190,80),Point(190,400)};
    fillPoly(maskL,pts2,Scalar(255));
    //maskL(Rect(0,0,220,480)) = 255;

    cv::Mat maskF = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
    vector<Point> pts1 = {Point(0,0),Point(640,0),Point(468,80),Point(190,80)};
    fillPoly(maskF,pts1,Scalar(255));
    //maskF(Rect(0,0,640,90)) = 255;

    cv::Mat maskR = cv::Mat::zeros(cv::Size(640, 480), CV_8U);
    vector<Point> pts3 = {Point(640,0),Point(640,480),Point(468,400),Point(468,80)};
    fillPoly(maskR,pts3,Scalar(255));
    //maskR(Rect(420,0,220,480)) = 255;

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

    while(1){

//        frontview=cv::imread("/home/cdukunlu/Qt_Files/calibration_Test/left-0000.png",0);
//        cv::imshow("Test",frontview);
        string one=getTimeStr();
//        std::cout<<one<<endl;

        //cap1>>test;

        cap1>>frame1;
        cap2>>frame2;
        cap3>>frame3;
        cap4>>frame4;


//        frame1=fileRead("/home/cdukunlu/Qt_Files/opencv_2 1/Calibration_Files/Front/ost.yaml",frame1); //Front
//        frame2=fileRead("/home/cdukunlu/Qt_Files/opencv_2 1/Calibration_Files/Left/ost.yaml",frame2); //Left
//        frame3=fileRead("/home/cdukunlu/Qt_Files/opencv_2 1/Calibration_Files/Right/ost.yaml",frame3); //Right
//        frame4=fileRead("/home/cdukunlu/Qt_Files/opencv_2 1/Calibration_Files/Back/ost.yaml",frame4); //Back

//        string ty =  type2str( frame1.type() );
//        printf("Matrix: %s %dx%d \n", ty.c_str(), frame1.cols, frame1.rows );

//        string ty1 =  type2str( frame2.type() );
//        printf("Matrix: %s %dx%d \n", ty1.c_str(), frame2.cols, frame2.rows );

//        string ty2 =  type2str( frame3.type() );
//        printf("Matrix: %s %dx%d \n", ty2.c_str(), frame3.cols, frame3.rows );

//        string ty3 =  type2str( frame4.type() );
//        printf("Matrix: %s %dx%d \n", ty3.c_str(), frame4.cols, frame4.rows );

//        cap1>>frame1original;
//        cap2>>frame2original;
//        cap3>>frame3original;
//        cap4>>frame4original;


        warpPerspective(frame1,frame1,F,Size(640,480));

        warpPerspective(frame2,frame2,L,Size(640,480));

        cv::rotate(frame2, frame2, cv::ROTATE_90_COUNTERCLOCKWISE);

        warpPerspective(frame3,frame3,R,Size(640,480));


        cv::rotate(frame3, frame3, cv::ROTATE_90_CLOCKWISE);

        warpPerspective(frame4,frame4,B,Size(640,480));


        cv::rotate(frame4, frame4, cv::ROTATE_180);


        resize(frame1, frame1, Size(640, 480), INTER_LINEAR);
        resize(frame2, frame2, Size(640, 480), INTER_LINEAR);
        resize(frame3, frame3, Size(640, 480), INTER_LINEAR);
        resize(frame4, frame4, Size(640, 480), INTER_LINEAR);



        bitwise_and(frame1,maskF,resF);
        bitwise_and(frame2,maskL,resL);
        bitwise_and(frame3,maskR,resR);
        bitwise_and(frame4,maskB,resB);



        texture_1=texture_1+resR+resL+resB+resF;
        resize(texture_1, resize_down, Size(down_width, down_height), INTER_LINEAR);

        GaussianBlur(resize_down, resize_down, Size(9, 9), 0);

        cv::putText(resize_down,
                    one,
                    cv::Point(0,475), // Coordinates (Bottom-left corner of the text string in the image)
                    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                    0.6, // Scale. 2.0 = 2x bigger
                    cv::Scalar(255,250,250), // BGR Color
                    1.0, // Line Thickness (Optional)
                    cv:: LINE_AA); // Anti-alias (Optional, see version note)


//        imshow("Front",frame1original);
//        imshow("Left",frame2original);
//        imshow("Right",frame3original);
//        imshow("Back",frame4original);

        imshow("Front1",resize_down);

        texture_1=0;


        if(waitKey(30) == 27) // Wait for 'esc' key press to exit
        {
            break;
        }

        //cout<<"Mask depth:"<<maskL.depth()<<", Frame depth:"<<frame3.depth()<<endl<<"Mask channels:"<<maskL.channels()<<",Frame depth:"<<frame3.channels()<<endl;
        //cout<<"Test: "<<maskL.size()<<","<<frame3.size()<<endl;

        setMouseCallback("Front", CallBackFunc, &frame1original);
        setMouseCallback("Left", CallBackFunc, &frame2original);
        setMouseCallback("Right", CallBackFunc, &frame3original);
        setMouseCallback("Back", CallBackFunc, &frame4original);

    }

    Point TestP;
    setMouseCallback("Stitched", CallBackFunc, &frame1original);

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

    destroyAllWindows();
}

int main()
{
    // The parameters to the function are put after the comma
    while (1)
    {
        BirdView();

    }
    return 0;
}




