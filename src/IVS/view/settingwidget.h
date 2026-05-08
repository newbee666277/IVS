#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QJsonArray>
#include <QLineEdit>
#include <QMouseEvent>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "../model/settinginfo.h"
#include "../service/logtask.h"
#include "../service/settinginfoservice.h"

class SettingWidget : public QWidget
{
    Q_OBJECT
private:
    QLabel* lab_title; //界面标题
    QLineEdit* edit_path; //选择的文件路径
    QComboBox* box_interval; //采集时间间隔下拉框
    QComboBox* box_channelIds[4]; //4个通道选择下拉框
    QLineEdit* edit_channel_names[4]; //4个通道名字输入框
    QPushButton* btn_path; //路径选择按钮
    QPushButton* btn_save; //保存设置按钮
    QPushButton* btn_cancel; //取消设置按钮
    QPushButton* btn_close_win; //关闭窗口按钮
    QPoint m_dragPos; //记录鼠标按下时的位置，用作移动窗口
    bool isDragging = false; //窗口是否被拖动
public:
    explicit SettingWidget(QWidget *parent = nullptr);
    void init_widget();
    void init_qss();
    void init_connect();
    static LogTask* logtask;
protected:
    void mousePressEvent(QMouseEvent* event) override; //鼠标按下事件
    void mouseMoveEvent(QMouseEvent* event) override;  //鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event) override; //鼠标松开事件
signals:

public slots:
    void checkInfoAndSave();
    void updatePathEdit();
};

#endif // SETTINGWIDGET_H
