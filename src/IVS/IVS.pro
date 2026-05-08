QT += core gui sql multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    model/camerainfo.cpp \
    model/exceptionloginfo.cpp \
    model/exceptionvideoinfo.cpp \
    model/featureinfo.cpp \
    model/featuremodel.cpp \
    model/logmodel.cpp \
    model/normalloginfo.cpp \
    model/normalvideoinfo.cpp \
    model/settinginfo.cpp \
    model/settinginfomodel.cpp \
    model/usermodel.cpp \
    model/videoinfomodel.cpp \
    model/videomodel.cpp \
    service/detecttask.cpp \
    service/featureservice.cpp \
    service/logtask.cpp \
    service/monitortask.cpp \
    service/recordtask.cpp \
    service/settinginfoservice.cpp \
    service/usersevice.cpp \
    service/videoplaytask.cpp \
    service/videoservice.cpp \
    util/dbconn.cpp \
    util/dbconnectionpool.cpp \
    util/windowmanager.cpp \
    view/datepicker.cpp \
    view/loginwidget.cpp \
    view/logwidget.cpp \
    view/mainwidget.cpp \
    view/monitorwidget.cpp \
    view/registerwidget.cpp \
    view/settingwidget.cpp \
    view/vcode.cpp \
    view/videotable.cpp \
    view/videowidget.cpp \
    yolo/Detect.cpp \
    yolo/YOLO.cpp


HEADERS += \
    model/camerainfo.h \
    model/exceptionloginfo.h \
    model/exceptionvideoinfo.h \
    model/featureinfo.h \
    model/featuremodel.h \
    model/logmodel.h \
    model/normalloginfo.h \
    model/normalvideoinfo.h \
    model/settinginfo.h \
    model/settinginfomodel.h \
    model/usermodel.h \
    model/videoinfomodel.h \
    model/videomodel.h \
    service/detecttask.h \
    service/featureservice.h \
    service/logtask.h \
    service/monitortask.h \
    service/recordtask.h \
    service/settinginfoservice.h \
    service/usersevice.h \
    service/videoplaytask.h \
    service/videoservice.h \
    util/dbconn.h \
    util/dbconnectionpool.h \
    util/windowmanager.h \
    view/datepicker.h \
    view/loginwidget.h \
    view/logwidget.h \
    view/mainwidget.h \
    view/monitorwidget.h \
    view/registerwidget.h \
    view/settingwidget.h \
    view/vcode.h \
    view/videotable.h \
    view/videowidget.h \
    yolo/Detect.h \
    yolo/YOLO.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

OPENCV_ROOT = D:/Develop/opencv

INCLUDEPATH += $${OPENCV_ROOT}/include
LIBS += -L$${OPENCV_ROOT}/x64/mingw/lib

LIBS += -llibopencv_core4100 # 核心模块（内存管理、数据结构）
LIBS += -llibopencv_imgproc4100 # 图像处理（滤波、缩放等）
LIBS += -llibopencv_videoio4100 # 视频 I/O（调用 FFmpeg 读写视频）
LIBS += -llibopencv_highgui4100 # 简单 GUI 显示（如 imshow）
LIBS += -llibopencv_imgcodecs4100 # 图像编解码（读写图片）
LIBS += -llibopencv_features2d4100 # 特征检测（如 SIFT/SURF）
LIBS += -llibopencv_calib3d4100 # 相机标定、3D 重建
LIBS += -llibopencv_objdetect4100 # 目标检测（如 Haar 级联）
LIBS += -llibopencv_dnn4100

RESOURCES += \
    resources.qrc

DESTDIR = $$PWD/bin