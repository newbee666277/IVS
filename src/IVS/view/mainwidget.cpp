#include "mainwidget.h"
#include "../util/windowmanager.h"

QString MainWidget::admin_name = "未登录"; //管理员名称默认为未登录
int MainWidget::admin_id = 0;
bool MainWidget::isNormalReview = true; //默认为普通回看
QLabel* MainWidget::lab_time = nullptr;

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    //this->login_widget = &(WindowManager::getInstance()->loginwidget);
    this->setWindowTitle("监控系统主界面");
    this->setMinimumSize(1200, 800);
    this->resize(1200, 800);

    //设置主窗口的主布局
    QVBoxLayout* main_layout = new QVBoxLayout;
    this->setLayout(main_layout);

    //顶部窗口布局
    top_layout = new QHBoxLayout;

    //底部窗口布局（堆叠布局）
    bottom_layout = new QStackedLayout;
    //初始化监控窗口
    monitor_widget = new MonitorWidget;
    bottom_layout->addWidget(monitor_widget);
    //初始化回放窗口
    VideoWidget* v_widget = new VideoWidget;
    bottom_layout->addWidget(v_widget);
    //初始化日志窗口
    LogWidget* l_widget = new LogWidget;
    bottom_layout->addWidget(l_widget);

    //设置默认为监控窗口
    bottom_layout->setCurrentIndex(0);

    main_layout->addLayout(top_layout, 1);
    main_layout->addLayout(bottom_layout, 8);
    main_layout->addStretch(2);

    //初始化顶部窗口
    this->init_top_layout();
    this->init_qss();

    this->init_conncect();

    this->init_timer();
}

void MainWidget::init_top_layout()
{
    //主标题
    QLabel* lab_title = new QLabel("智能监控系统");
    lab_title->setObjectName("title");

    //按钮
    btn_monitor = new QPushButton(QIcon(":/images/monitor.png"),"监控");
    btn_normal_video = new QPushButton(QIcon(":/images/video.png"),"普通回放");
    btn_exception_video = new QPushButton(QIcon(":/images/video.png"),"异常回放");
    btn_log_review = new QPushButton(QIcon(":/images/log.png"),"日志查看");
    btn_set = new QPushButton(QIcon(":/images/set.png"),"设置");

    if(MainWidget::admin_name == "未登录"){
        btn_normal_video->setEnabled(false);
        btn_exception_video->setEnabled(false);
        btn_log_review->setEnabled(false);
    }else{
        btn_normal_video->setEnabled(true);
        btn_exception_video->setEnabled(true);
        btn_log_review->setEnabled(true);
    }

    //时间
    lab_time = new QLabel("2026-3-12");
    lab_time->setObjectName("ctime");

    //登录状态
    QWidget* log_widget = new QWidget;
    QVBoxLayout* log_layout = new QVBoxLayout;
    log_widget->setLayout(log_layout);
    //上方按钮
    btn_log = new QPushButton("");
    btn_log->setIcon(QIcon(":/images/register.png"));
    btn_log->setIconSize(QSize(40,60));
    //下方文字
    btn_log_lab = new QLabel("未登录");
    btn_log_lab->setAlignment(Qt::AlignCenter);
    btn_log_lab->setObjectName("log_lab");
    log_layout->addWidget(btn_log);
    log_layout->addWidget(btn_log_lab);

    //登录状态和文字用垂直布局
    QVBoxLayout* log_and_time_layout = new QVBoxLayout;
    log_and_time_layout->addWidget(lab_time);
    log_and_time_layout->addWidget(log_widget);

    this->top_layout->addWidget(lab_title);
    this->top_layout->addStretch(1);
    this->top_layout->addWidget(btn_monitor);
    this->top_layout->addStretch(1);
    this->top_layout->addWidget(btn_normal_video);
    this->top_layout->addStretch(1);
    this->top_layout->addWidget(btn_exception_video);
    this->top_layout->addStretch(1);
    this->top_layout->addWidget(btn_log_review);
    this->top_layout->addStretch(1);
    this->top_layout->addWidget(btn_set);
    this->top_layout->addStretch(1);
    this->top_layout->addLayout(log_and_time_layout);
    // this->top_layout->addWidget(lab_time);
    // this->top_layout->addWidget(log_widget);

}

void MainWidget::init_qss()
{
    QFile file(":/qss/top_window.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void MainWidget::init_conncect()
{
    //登录界面与主界面间切换
    connect(this->btn_log, SIGNAL(clicked()), this, SLOT(move_to_login()));
    //connect(login_widget, SIGNAL(login_success(QString)), this, SLOT(back_to_main(QString)));

    //主界面切换到设置界面
    connect(btn_set, &QPushButton::clicked, this, &MainWidget::move_to_setting);

    //主界面中监控和回放窗口切换
    connect(btn_monitor, &QPushButton::clicked, this, [this](){
        this->switch_monitor();
        SettingWidget::logtask->recordLog(MainWidget::admin_id, MainWidget::admin_name, lab_time->text(), "查看监控");
    });
    connect(btn_normal_video, &QPushButton::clicked, this, [this](){
        normal_btn_clicked();
        SettingWidget::logtask->recordLog(MainWidget::admin_id, MainWidget::admin_name, lab_time->text(), "查看普通回放");
    });
    connect(btn_exception_video, &QPushButton::clicked, this, [this](){
        exception_btn_clicked();
        SettingWidget::logtask->recordLog(MainWidget::admin_id, MainWidget::admin_name, lab_time->text(), "查看异常回放");
    });
    connect(this, &MainWidget::normal_clicked, this, &MainWidget::switch_video);
    connect(this, &MainWidget::exception_clicked, this, &MainWidget::switch_video);

    //切换至日志窗口
    connect(btn_log_review, &QPushButton::clicked, this, [this](){
        this->bottom_layout->setCurrentIndex(2);
        SettingWidget::logtask->recordLog(MainWidget::admin_id, MainWidget::admin_name, lab_time->text(), "查看日志");
    });
}

void MainWidget::showWidget(QString &str)
{
    this->show();
    this->btn_log_lab->setText(str);

    if(MainWidget::admin_name == "未登录"){
        btn_normal_video->setEnabled(false);
        btn_exception_video->setEnabled(false);
        btn_log_review->setEnabled(false);
    }else{
        btn_normal_video->setEnabled(true);
        btn_exception_video->setEnabled(true);
        btn_log_review->setEnabled(true);

        // 登录成功后，重新启动当前监控，使 RecordTask 被创建并连接
        if(monitor_widget != nullptr){
            monitor_widget->restartMonitor(1);
        }
    }
}



void MainWidget::update_time()
{
    QDateTime current_time = QDateTime::currentDateTime();
    QString time_string = current_time.toString("yyyy-MM-dd HH:mm:ss");

    this->lab_time->setText(time_string);
}

void MainWidget::init_timer()
{
    update_time(); //启动时就更新一次时间

    QTimer* m_timer = new QTimer;
    m_timer->setInterval(1000); //设置时间间隔为1秒

    connect(m_timer, SIGNAL(timeout()), this, SLOT(update_time()));

    m_timer->start(); //启动计时器

}

void MainWidget::switch_monitor()
{
    bottom_layout->setCurrentIndex(0);
}

void MainWidget::switch_video(QString message)
{
    bottom_layout->setCurrentIndex(1);
    if(message == "普通回放"){
        qDebug() << "切换至普通";
        MainWidget::isNormalReview = true;
    }else if(message == "异常回放"){
        qDebug() << "切换至异常";
        MainWidget::isNormalReview = false;
    }

    VideoWidget* vw = qobject_cast<VideoWidget*>(bottom_layout->widget(1));
    if(vw) {
        // 调用 date_change 会自动把页码重置为 1，并根据最新的 isNormalReview 状态重新查库
        vw->data_change();
    }

}

void MainWidget::move_to_setting()
{
    WindowManager::getInstance()->settingwidget.show();
    this->hide();
}

void MainWidget::normal_btn_clicked()
{
    QString message = this->btn_normal_video->text();
    emit normal_clicked(message);
}

void MainWidget::exception_btn_clicked()
{
    QString message = this->btn_exception_video->text();
    emit exception_clicked(message);
}

void MainWidget::move_to_login()
{
    WindowManager::getInstance()->loginwidget.show();
    this->hide();
}

//void MainWidget::back_to_main(QString username)
//{
//    //this->login_widget->show();
//    login_widget->hide();
//    this->btn_log_lab->setText(username);
//    this->show();
//}


