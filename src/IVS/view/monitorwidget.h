#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFile>
#include <QIcon>
#include <QThread>
#include <QList>
#include <QApplication>
#include <QCheckBox>
#include <QTimer> // 【新增】
#include <mutex>  // 【新增】用于跨线程加锁
#include "../service/monitortask.h"
#include "../service/recordtask.h"
#include "../service/detecttask.h"
#include "../model/settinginfo.h"
#include "../service/featureservice.h"

class MonitorWidget : public QWidget
{
    Q_OBJECT
private:
    QWidget* left_widget;
    QWidget* right_widget;
    QPushButton* btn_w1;
    QPushButton* btn_w4;
    QPushButton* btn_capture_image; //截图按钮
    QCheckBox* checkbox_detect; //是否开启移动检测的勾选框
    QLabel* lab_monitor;
    QLabel* lab_monitors[4];
    QStackedLayout* monitor_layout;
    //QImage imgs[4];
    QList<MonitorTask*> m_tasks_list;
    QList<QThread*> m_thread_list;
    QList<RecordTask*> r_tasks_list;
    QList<DetectTask*> d_tasks_list;
    QList<QThread*> d_threads_list;
    QString monitor_path; //各个通道的路径
    QListWidget* list_devices;
    bool isSwitching = false; //是否正在切换通道

    QThread* r_thread = nullptr; //只创建一个记录线程

    cv::Mat latest_frames[4]; // 【优化】专门存放 4 个通道的最新帧
    std::mutex frame_mutex;   // 【优化】保护最新帧的互斥锁
    QTimer* render_timer;     // 【优化】控制UI主线程渲染的定时器

    int current_camera_count = 1;
public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    void init_left_widget();
    void init_right_widget();
    void init_qss();
    void init_connect();
    void startMonitor(int camera_count);
    QImage MatToQImage(Mat &frame);
    //void paintEvent(QPaintEvent *event);
    void stopAllTasks();
    static bool isDetect; //是否在检测
    void updateList();
    void restartMonitor(int camera_count);
signals:

public slots:
    void switch_channel1();
    void switch_channel4();
    void renderFrames(); // 【优化】：主线程定时拉取并渲染画面

};

#endif // MONITORWIDGET_H
