#include "settingwidget.h"
#include "../util/windowmanager.h"
#include "../service/logtask.h"

LogTask* SettingWidget::logtask = new LogTask;

SettingWidget::SettingWidget(QWidget *parent)
    : QWidget{parent}
{
    this->init_widget();

    this->init_qss();

    this->init_connect();

    //最开始直接判断路径是否合法
    updatePathEdit();

    QThread* l_thread = new QThread;
    SettingWidget::logtask->moveToThread(l_thread);
}

void SettingWidget::init_widget()
{
    this->setFixedSize(800,600);
    //关闭自带的窗口移动，拖拽，放大缩小
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    this->setFont(QFont("华为新魏",16,400));
    this->setContentsMargins(100,10,100,10);
    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->setSpacing(20);
    this->setLayout(main_layout);

    //标题和自定义关闭窗口按钮
    QHBoxLayout* title_layout = new QHBoxLayout;
    lab_title = new QLabel("智能监控系统设置界面");
    lab_title->setObjectName("titleLabel");
    //lab_title->setAlignment(Qt::Alignment);
    btn_close_win = new QPushButton();
    btn_close_win->setIcon(QIcon(":/images/close.png"));
    btn_close_win->setFixedSize(36, 36);
    connect(btn_close_win, &QPushButton::clicked, this, [this](){
        auto res = QMessageBox::question(this, "确认", "是否关闭窗口");
        if(res == QMessageBox::Yes){
            this->close();
        }

    });
    title_layout->addSpacing(36);
    title_layout->addStretch();
    title_layout->addWidget(lab_title);
    title_layout->addStretch();
    title_layout->addWidget(btn_close_win);

    //文件保存路径
    QHBoxLayout* path_layout = new QHBoxLayout;
    QLabel* lab_path = new QLabel("保存路径: ");
    edit_path = new QLineEdit;
    edit_path->setReadOnly(true);
    edit_path->setText(SettingInfo::filepath);
    btn_path = new QPushButton("浏览");
    path_layout->addWidget(lab_path);
    path_layout->addWidget(edit_path);
    path_layout->addWidget(btn_path);

    //录制间隔
    QHBoxLayout* interval_layout = new QHBoxLayout;
    QLabel* lab_interval = new QLabel("间隔时间: ");
    box_interval = new QComboBox;
    box_interval->addItem("30秒", 30);
    box_interval->addItem("60秒", 60);
    box_interval->addItem("120秒", 120);
    int interval_time = SettingInfo::timeInterval == 0? 30: SettingInfo::timeInterval;
    box_interval->setCurrentText(QString::number(interval_time) + "秒");
    interval_layout->addWidget(lab_interval);
    interval_layout->addWidget(box_interval);
    interval_layout->addStretch();

    //通道名
    QVBoxLayout* channel_layout = new QVBoxLayout;
    for(int i = 0; i < SettingInfo::cameras.size(); i++){
        QHBoxLayout* channel_h_layout = new QHBoxLayout;
        QLabel* camera_name = new QLabel;
        camera_name->setText(SettingInfo::cameras[i].getCamera_name());
        edit_channel_names[i] = new QLineEdit;
        edit_channel_names[i]->setText(SettingInfo::cameras[i].getChannel_name());
        box_channelIds[i] = new QComboBox;
        box_channelIds[i]->addItem("通道1",1);
        box_channelIds[i]->addItem("通道2",2);
        box_channelIds[i]->addItem("通道3",3);
        box_channelIds[i]->addItem("通道4",4);
        box_channelIds[i]->setCurrentIndex(SettingInfo::cameras[i].getChannel_id() - 1);
        channel_h_layout->addWidget(camera_name);
        channel_h_layout->addWidget(edit_channel_names[i]);
        channel_h_layout->addWidget(box_channelIds[i]);
        channel_layout->addLayout(channel_h_layout);
    }

    //按钮
    QHBoxLayout* btn_layout = new QHBoxLayout;
    btn_save = new QPushButton("保存");
    btn_cancel = new QPushButton("取消");
    btn_layout->addWidget(btn_save);
    btn_layout->addWidget(btn_cancel);

    main_layout->addLayout(title_layout);
    main_layout->addStretch();
    main_layout->addLayout(path_layout);
    main_layout->addStretch();
    main_layout->addLayout(interval_layout);
    main_layout->addStretch();
    main_layout->addLayout(channel_layout);
    main_layout->addStretch();
    main_layout->addLayout(btn_layout);
    main_layout->addStretch();
}

void SettingWidget::init_qss()
{
    QFile file("../qss/settingwidget.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void SettingWidget::init_connect()
{
    connect(btn_path, &QPushButton::clicked, this, [this](){
        //弹出目录选择对话框
        QString selectDirPath = QFileDialog::getExistingDirectory(this, tr("请选择目标目录"), "./");

        //判断是否选择目录成功
        if(!selectDirPath.isEmpty()){
            qDebug() << "选中路径为: " <<selectDirPath;
            //业务逻辑: 存储到数据库中

            //显示到输入框
            this->edit_path->setText(selectDirPath);
            //强制显示开头内容
            this->edit_path->setCursorPosition(0);
            //鼠标悬停显示完整目录路径
            this->edit_path->setToolTip(selectDirPath);
        }else{
            qDebug() << "用户取消选择";
        }
    });

    connect(btn_save, &QPushButton::clicked, this, [this](){

        QString path = edit_path->text();

        if(path.isEmpty() || !QDir(path).exists()){
            QMessageBox::warning(this, "错误", "路径无效,请重新选择");
            edit_path->setStyleSheet("border: 2px solid red;");
            return;
        }

    });

    connect(edit_path, &QLineEdit::textChanged, this, &SettingWidget::updatePathEdit);
    connect(btn_save, &QPushButton::clicked, this, &SettingWidget::checkInfoAndSave);
    connect(btn_cancel, &QPushButton::clicked, this, [this](){
        auto res = QMessageBox::question(this,"提示","确定要退出吗？");
        if(res == QMessageBox::Yes){
            this->close();
        }
    });
}

void SettingWidget::mousePressEvent(QMouseEvent *event)
{
    //只响应鼠标左键
    if(event->button() == Qt::LeftButton){
        if(this->isMaximized() || this->isFullScreen()){ //最大化或全屏时不允许拖动
            return;
        }

        isDragging = true;
        m_dragPos = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }

}

void SettingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(isDragging && event->buttons() == Qt::LeftButton){
        this->move(event->globalPos() - m_dragPos);
        event->accept();
    }
}

void SettingWidget::mouseReleaseEvent(QMouseEvent *event)
{
    isDragging = false;
    event->accept();
}

void SettingWidget::checkInfoAndSave()
{
    bool flag = false;
    //检查通道是否重复
    std::unordered_map<int, int> map;
    for(auto box_id : this->box_channelIds){
        int id = box_id->currentData().toInt();
        auto it = map.find(id);
        if(it != map.end()){
            map[id]++;
        }else{
            map[id] = 1;
        }
    }

    for(auto it = map.begin(); it != map.end(); it++){
        if(it->second > 1){
            QMessageBox::warning(this, "错误", "通道重复,请重新选择通道");
            return;
        }
    }

    flag = true;

    //通道合法，检查路径
    if(flag){
        QString path = edit_path->text();

        if(path.isEmpty() || !QDir(path).exists()){
            QMessageBox::warning(this, "错误", "路径无效,请重新选择");
            edit_path->setStyleSheet("border: 2px solid red;");
            return;
        }
    }

    //路径也合法，选择是否保存
    if(flag){
        auto result = QMessageBox::question(this, "配置保存确认", "是否确认保存");
        if(result == QMessageBox::Yes){
            SettingInfo::filepath = edit_path->text();
            SettingInfo::timeInterval = box_interval->currentData().toInt();
            for(int i = 0; i < SettingInfo::cameras.size(); i++){
                SettingInfo::cameras[i].setChannel_name(edit_channel_names[i]->text());
                SettingInfo::cameras[i].setChannel_id(box_channelIds[i]->currentData().toInt());
            }

            //写入文件
            SettingInfo::saveToFile();
            //写入数据库
            SettingInfoService sis;
            sis.saveSettingToDb();

            this->hide();
            WindowManager::getInstance()->mainwidget.show();
        }
    }


}

void SettingWidget::updatePathEdit()
{
    QString path = edit_path->text();

    if(path.isEmpty() || !QDir(path).exists()){
        //错误
        edit_path->setStyleSheet("border: 2px solid red;");
    }else{
        //正常
        edit_path->setStyleSheet("");
    }
}


