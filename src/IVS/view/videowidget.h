#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QFile>
#include <QIcon>
#include <QComboBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSlider>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <QRadioButton>
#include <QScrollArea>
#include "../view/settingwidget.h"
#include "../model/featuremodel.h"
#include "../model/featureinfo.h"
#include "../util/dbconn.h"
#include "./service/videoservice.h"
#include "datepicker.h"
#include "../model/normalvideoinfo.h"
#include "../model/exceptionvideoinfo.h"
#include "../service/videoplaytask.h"
#include "../service/featureservice.h"


class VideoWidget : public QWidget
{
    Q_OBJECT
private:
    QWidget* left_widget;
    QWidget* right_widget;
    QLabel* lab_video;
    QVBoxLayout* video_layout;
    QPushButton *btn_pre;
    QPushButton *btn_next;
    QLabel *lab_page;
    QComboBox* dates_com;
    QComboBox* channel_com;
    QListWidget* list_video;
    QString selected_date;
    DatePicker* datepicker;
    QThread *v_cur_thread = nullptr; //当前正在运行的视频播放线程
    VideoPlayTask *v_task = nullptr;
    QSlider* video_slider; //视频下方进度条
    QPushButton* btn_ctr; //视频播放/暂停控制按钮
    QComboBox* box_speed; //倍速选择下拉框
    QPushButton* btn_capture_img; //截图按钮
    QImage img;
    QPushButton* btn_video_opt; //选择视频预览
    QPushButton* btn_img_opt; //选择图片预览
    QStackedLayout* right_stacked_layout; // 控制右侧内容的堆叠布局
    QLabel* lab_image_display; // 用于全屏显示图片的标签
    QPushButton* btn_related_video; //查询截图相关视频
    QPushButton* btn_img_expand; //放大图片
    QPushButton* btn_img_reduce; //缩小图片
    QSlider* slider_img_size; //图像大小进度条
    QPushButton* btn_img_fitwin; //截图回到正常大小
    QString path; //查询的图片或视频路径
    int current_page;
    int page_size;
    int total_records;
    int total_pages;
    int cur_video_channel_id; //当前播放的视频channel_id
    bool isVideoOpt = true; //判断是否是查询视频-默认为true
    QScrollArea* image_scroll_area; //滚动区域 实现图片放大与缩小
    QPixmap base_image_pixmap; //存储适应窗口时的基础画质图片

public:
    explicit VideoWidget(QWidget *parent = nullptr);
    void init_left_widget();
    void init_right_widget();
    //void init_dates_combo();
    void update_list();
    QImage MatToQImage(cv::Mat& frame);
    void init_qss();
    void init_connect();
    void paintEvent(QPaintEvent *event);
    void stopCurVideoPlayThread(); //停掉当前正在运行的视频播放线程
signals:

public slots:
    void pre_page();
    void next_page();
    void data_change();
    //void channel_change();
    void playVideo(QListWidgetItem*);
};

#endif // VIDEOWIDGET_H
