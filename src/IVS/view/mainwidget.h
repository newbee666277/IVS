#ifndef MAINWIDGET_H
#define MAINWIDGET_H

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
#include <QPalette>
#include <QDateTime>
#include <QTimer>
#include <QPixmap>
#include <QVariant>
#include "monitorwidget.h"
#include "videowidget.h"
#include "loginwidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout* top_layout;
    QStackedLayout* bottom_layout;
    QPushButton* btn_log;
    QLabel* btn_log_lab;
    //loginWidget* login_widget;
    QPushButton* btn_monitor;
    QPushButton* btn_normal_video;
    QPushButton* btn_exception_video;
    QPushButton* btn_set;
    QPushButton* btn_log_review; //日志查看
    MonitorWidget* monitor_widget = nullptr;
public:
    explicit MainWidget(QWidget *parent = nullptr);
    void init_top_layout();
    void init_qss();
    void init_conncect();
    void showWidget(QString& str);
    static bool isLogin; //判断登录状态
    static QString admin_name; //记录当前的管理员名称
    static int admin_id; //当前管理员ID
    static bool isNormalReview; //记录当前查看的是普通回放还是异常回放，因为两个按钮共用一个界面，true-普通，false-异常
    static QLabel* lab_time;
public slots:
    void move_to_login();
    //void back_to_main(QString);
    void update_time();
    void init_timer();
    void switch_monitor();
    void switch_video(QString message);
    void move_to_setting();
    void normal_btn_clicked(); //普通回看按钮被点击
    void exception_btn_clicked(); //异常回看按钮被点击

signals:
    void normal_clicked(QString);
    void exception_clicked(QString);
};

#endif // MAINWIDGET_H
