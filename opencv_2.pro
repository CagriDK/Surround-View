QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv4

LIBS += -L"/home/cdukunlu/opencv_build/opencv/build/lib"\
-lopencv_stitching\
-lopencv_aruco\
-lopencv_bgsegm\
-lopencv_bioinspired\
-lopencv_ccalib\
-lopencv_dnn_objdetect\
-lopencv_dnn_superres\
-lopencv_dpm\
-lopencv_highgui\
-lopencv_face\
-lopencv_freetype\
-lopencv_fuzzy\
-lopencv_hfs\
-lopencv_img_hash\
-lopencv_line_descriptor\
-lopencv_quality\
-lopencv_reg\
-lopencv_rgbd\
-lopencv_saliency\
-lopencv_shape\
-lopencv_stereo\
-lopencv_structured_light\
-lopencv_phase_unwrapping\
-lopencv_superres\
-lopencv_optflow\
-lopencv_surface_matching\
-lopencv_tracking\
-lopencv_datasets\
-lopencv_text\
-lopencv_dnn\
-lopencv_plot\
-lopencv_ml\
-lopencv_videostab\
-lopencv_videoio\
-lopencv_viz\
-lopencv_ximgproc\
-lopencv_video\
-lopencv_xobjdetect\
-lopencv_objdetect\
-lopencv_calib3d\
-lopencv_imgcodecs\
-lopencv_features2d\
-lopencv_flann\
-lopencv_xphoto\
-lopencv_photo\
-lopencv_imgproc\
-lopencv_core\
-lopencv_xfeatures2d
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
