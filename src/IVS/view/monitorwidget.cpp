#include "monitorwidget.h"
#include "settingwidget.h"
#include "mainwidget.h"

bool MonitorWidget::isDetect = false;

MonitorWidget::MonitorWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* main_layout = new QHBoxLayout;
    this->setLayout(main_layout);

    //监控窗口分为左右两个盒子
    left_widget = new QWidget;
    right_widget = new QWidget;
    main_layout->addWidget(left_widget, 1);
    main_layout->addWidget(right_widget, 8);

    this->init_right_widget();
    this->init_left_widget();

    this->init_qss();

    this->init_connect();

    //默认为单通道
    this->startMonitor(1);

    // 【优化】：初始化UI主线程的定时器，设置为 33ms (约 30FPS)
    render_timer = new QTimer(this);
    connect(render_timer, &QTimer::timeout, this, &MonitorWidget::renderFrames);
    render_timer->start(33);

    //检测到退出系统
    connect(qApp, &QApplication::aboutToQuit, this, &MonitorWidget::stopAllTasks);
}

void MonitorWidget::init_left_widget()
{
    QVBoxLayout* v_layout = new QVBoxLayout;
    this->left_widget->setLayout(v_layout);
    this->left_widget->setObjectName("left_box");


    //标题
    QLabel* lab_devices = new QLabel("设备列表");
    lab_devices->setObjectName("lab_devices_title");

    //列表
    list_devices = new QListWidget;
    list_devices->setObjectName("device_list");
    updateList();

    v_layout->addWidget(lab_devices);
    v_layout->addWidget(list_devices);
}

void MonitorWidget::init_right_widget()
{
    //右盒子上方为监控视频，下方为按钮，设置为垂直布局
    QVBoxLayout* v_layout = new QVBoxLayout;
    right_widget->setLayout(v_layout);

    //上方监控采用堆叠布局
    monitor_layout = new QStackedLayout;

    //创建单通道窗口
    QWidget* widget_w1 = new QWidget;
    monitor_layout->addWidget(widget_w1);

    //初始化单通道窗口
    QVBoxLayout* w1_layout = new QVBoxLayout;
    widget_w1->setLayout(w1_layout);
    lab_monitor = new QLabel;
    lab_monitor->setPixmap(QPixmap(":/images/no_camera.png"));
    lab_monitor->setScaledContents(true); //设置内容随窗口缩放
    lab_monitor->setAlignment(Qt::AlignCenter); //设置居中显示
    w1_layout->addWidget(lab_monitor);

    //创建四通道窗口
    QWidget* widget_w4 = new QWidget;
    monitor_layout->addWidget(widget_w4);

    //初始化四通道窗口
    QGridLayout* w4_layout = new QGridLayout; //四通道窗口用网格布局
    widget_w4->setLayout(w4_layout);
    int x = 0, y = 0; //网格布局的坐标
    for(int i = 0; i < 4; i++){
        lab_monitors[i] = new QLabel;
        lab_monitors[i]->setPixmap(QPixmap(":/images/no_camera.png"));
        lab_monitors[i]->setScaledContents(true); //设置内容随窗口缩放
        lab_monitors[i]->setAlignment(Qt::AlignCenter); //设置居中显示
        switch(i){
            case 0: x = y = 0; break;
            case 1: x = 0; y = 1; break;
            case 2: x = 1; y = 0; break;
            case 3: x = y = 1; break;
        }
        w4_layout->addWidget(lab_monitors[i], x, y);
    }

    //默认为单通道窗口
    monitor_layout->setCurrentIndex(0);

    //下方按钮采用水平布局
    QHBoxLayout* btn_layout = new QHBoxLayout;
    btn_w1 = new QPushButton(QIcon(":/images/w1.png"), "");
    btn_w4 = new QPushButton(QIcon(":/images/w4.png"), "");
    btn_capture_image = new QPushButton("截图");
    btn_w1->setObjectName("btn_channel_1");
    btn_w4->setObjectName("btn_channel_4");
    btn_capture_image->setObjectName("btn_capture_img");
    //目标检测勾选框
    checkbox_detect = new QCheckBox("移动目标检测", this);
    connect(checkbox_detect, &QCheckBox::checkStateChanged, this, [this](int state){
        this->isDetect = (state == Qt::Checked);
        SettingWidget::logtask->recordLog(MainWidget::admin_id, MainWidget::admin_name, MainWidget::lab_time->text(), isDetect ? "开启移动检测" : "关闭移动检测");
    });
    btn_layout->addWidget(btn_w1);
    btn_layout->addWidget(btn_w4);
    btn_layout->addWidget(btn_capture_image);
    btn_layout->addStretch();
    btn_layout->addWidget(checkbox_detect);

    v_layout->addLayout(monitor_layout, 8);
    v_layout->addLayout(btn_layout, 1);

}

void MonitorWidget::init_qss()
{
    QFile file(":/qss/monitorwidget.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void MonitorWidget::init_connect()
{
    connect(btn_w1, SIGNAL(clicked()), this, SLOT(switch_channel1()));
    connect(btn_w4, SIGNAL(clicked()), this, SLOT(switch_channel4()));
    //截图按钮
    connect(btn_capture_image, &QPushButton::clicked, this, [this](){
        //先安全地获取当前瞬间的 QImage
        QImage cap_imgs[4];
        {
            std::lock_guard<std::mutex> lock(this->frame_mutex);
            for(int i = 0; i < 4; i++){
                if(!this->latest_frames[i].empty()){
                    cap_imgs[i] = MatToQImage(this->latest_frames[i]);
                }
            }
        }

        FeatureService fs;
        QString file_path = "";
        QString name = "";
        if(monitor_layout->currentIndex() == 0){
            if(fs.capture(cap_imgs[0], 1, file_path, name, false)){
                FeatureInfo f;
                FeatureModel fm;
                f.setName(name);
                f.setPath(file_path);
                f.setChannel_id(1);
                f.setException_id(0);
                f.setFeature_type(0);
                fm.insertFeatureInfo(f);
                QMessageBox::information(this, "提示", "截图已保存至" + file_path);
            }else{
                QMessageBox::warning(this, "警告", "截图保存失败");
            }
        }else if(monitor_layout->currentIndex() == 1){
            int flagcount = 0;
            for(int i = 0; i < 4; i++){
                if(fs.capture(cap_imgs[i], i+1, file_path, name, false)){
                    flagcount++;
                    FeatureInfo f;
                    FeatureModel fm;
                    f.setName(name);
                    f.setPath(file_path);
                    f.setChannel_id(i+1);
                    f.setException_id(0);
                    f.setFeature_type(0);
                    fm.insertFeatureInfo(f);
                }else{
                    QMessageBox::warning(this, "警告", "通道" + QString::number(i+1) + "截图保存失败");
                }
            }
            if(flagcount == 4){
                QMessageBox::information(this, "提示", "截图保存成功");
            }
        }
        SettingWidget::logtask->recordLog(MainWidget::admin_id, MainWidget::admin_name, MainWidget::lab_time->text(), "截图");
    });
}

void MonitorWidget::startMonitor(int camera_count)
{
    int video_index = 1;

    for(int i = 0; i < camera_count; i++){
        int target_channelid = i + 1;
        for(int j = 0; j < SettingInfo::cameras.size(); j++){
            if(SettingInfo::cameras[j].getCamera_name() != "virutal_camera"){ //真实摄像头
                if(SettingInfo::cameras[j].getChannel_id() == target_channelid){
                    monitor_path = nullptr;
                }
            }else{
                if(SettingInfo::cameras[j].getChannel_id() == target_channelid){
                    monitor_path = "data/video/pg" + QString::number(video_index++) + ".mp4";
                }
            }
        }

        //创建摄像头实时监控线程以及task类
        QThread *m_thread = new QThread;
        MonitorTask *m_task = new MonitorTask(monitor_path, i);
        m_task->moveToThread(m_thread);
        m_tasks_list.append(m_task);
        m_thread_list.append(m_thread);

        //创建目标检测线程以及task类
        QThread *d_thread = new QThread;
        DetectTask* d_task = new DetectTask;
        d_task->moveToThread(d_thread);
        d_tasks_list.append(d_task);
        d_threads_list.append(d_thread);

        connect(m_thread, &QThread::started, m_task, &MonitorTask::startMonitor);
        connect(m_task, &MonitorTask::sendFrame, d_task, &DetectTask::beginDectect);
        // connect(d_task, &DetectTask::sendDetectFrame, this, [this](Mat frame, int channel_id){
        //     this->imgs[channel_id] = MatToQImage(frame);
        //     this->update(); //需要重写update()
        // });

        //【优化】
        //使用共享变量存储发来的frame，以前是发送来就处理，现在是存储起来，
        //主线程每隔一段时间去处理，即使主线程卡住，卡住的那段时间发来的frame会被直接扔掉，不会堆积
        connect(d_task, &DetectTask::sendDetectFrame, this, [this](Mat frame, int channel_id){
            if(channel_id < 0 || channel_id >= 4){
                qDebug() << "非法 channel_id:" << channel_id;
                return;
            }

            std::lock_guard<std::mutex> lock(this->frame_mutex);
            this->latest_frames[channel_id] = frame.clone();
        });

        //只有登录后才能开始记录
        if(MainWidget::admin_name != "未登录"){
            //只创建一个记录线程
            if(r_thread == nullptr){
                r_thread = new QThread;
                //记录线程死掉的时候自己清理数据库连接
                connect(r_thread, &QThread::finished, r_thread, [](){
                    DbConnectionPool::getInstance()->releaseConnection();
                }, Qt::DirectConnection);
                r_thread->start();
            }
            //创建记录task类
            RecordTask *r_task = new RecordTask(m_task->getFps());
            r_task->moveToThread(r_thread);
            r_tasks_list.append(r_task);
            //连接记录信号和槽
            connect(d_task, &DetectTask::sendDetectFrame, r_task, &RecordTask::startRecord);

        }
        //启动线程
        m_thread->start();
        d_thread->start();
    }
}

QImage MonitorWidget::MatToQImage(Mat& frame)
{
    if(frame.empty()){
        return QImage();
    }

    switch(frame.type()){
        case CV_8UC1:{
            return QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_Grayscale8).copy();
        }
        case CV_8UC3:{
            cv::Mat rgb_frame; // 创建一个临时的 Mat
            // 把转换后的结果存到 rgb_frame里
            cv::cvtColor(frame, rgb_frame, cv::COLOR_BGR2RGB);
            return QImage(rgb_frame.data, rgb_frame.cols, rgb_frame.rows, rgb_frame.step, QImage::Format_RGB888).copy();
        }
        default:{
            return QImage();
        }
    }
}

// void MonitorWidget::paintEvent(QPaintEvent *event)
// {

//     if(this->monitor_layout->currentIndex() == 0){
//         QPixmap pixmap = QPixmap::fromImage(this->imgs[0]);
//         this->lab_monitor->setPixmap(pixmap);
//     }else{
//         for(int i = 0; i < 4; i++){
//             QPixmap pixmap = QPixmap::fromImage(this->imgs[i]);
//             lab_monitors[i]->setPixmap(pixmap);
//         }
//     }

// }

void MonitorWidget::stopAllTasks()
{
    // 1. 暂停 UI 渲染，防止切换过程中还在读 latest_frames
    if(render_timer != nullptr){
        render_timer->stop();
    }

    // 2. 先断开所有 DetectTask 的信号
    // 防止旧 DetectTask 继续把帧发给 UI 或 RecordTask
    for(int i = 0; i < d_tasks_list.size(); i++){
        if(d_tasks_list[i] != nullptr){
            disconnect(d_tasks_list[i], nullptr, nullptr, nullptr);
        }
    }

    // 3. 先断开 MonitorTask 的信号
    for(int i = 0; i < m_tasks_list.size(); i++){
        if(m_tasks_list[i] != nullptr){
            disconnect(m_tasks_list[i], nullptr, nullptr, nullptr);
        }
    }

    // 4. 停止视频源线程
    for(int i = 0; i < m_tasks_list.size(); i++){
        if(m_tasks_list[i] != nullptr){
            m_tasks_list[i]->stop();
        }

        if(i < m_thread_list.size() && m_thread_list[i] != nullptr){
            m_thread_list[i]->quit();
            m_thread_list[i]->wait();
        }

        delete m_tasks_list[i];
        if(i < m_thread_list.size()){
            delete m_thread_list[i];
        }
    }

    m_tasks_list.clear();
    m_thread_list.clear();

    // 5. 停止检测线程
    for(int i = 0; i < d_tasks_list.size(); i++){
        // 如果 DetectTask 类里面有 stop() 函数，就打开这一句
        // d_tasks_list[i]->stop();

        if(i < d_threads_list.size() && d_threads_list[i] != nullptr){
            d_threads_list[i]->quit();
            d_threads_list[i]->wait();
        }

        delete d_tasks_list[i];
        if(i < d_threads_list.size()){
            delete d_threads_list[i];
        }
    }

    d_tasks_list.clear();
    d_threads_list.clear();

    // 6. 停止录像任务
    // 这里先 stopRecord，再删除
    for(int i = 0; i < r_tasks_list.size(); i++){
        if(r_tasks_list[i] != nullptr){
            r_tasks_list[i]->stopRecord();
            delete r_tasks_list[i];
        }
    }

    r_tasks_list.clear();

    if(r_thread != nullptr){
        r_thread->quit();
        r_thread->wait();
        delete r_thread;
        r_thread = nullptr;
    }

    // 7. 清空旧帧，避免单通道/四通道切换后读到旧数据
    {
        std::lock_guard<std::mutex> lock(this->frame_mutex);
        for(int i = 0; i < 4; i++){
            latest_frames[i].release();
        }
    }

    // 8. 恢复 UI 渲染
    if(render_timer != nullptr){
        render_timer->start(33);
    }
}

void MonitorWidget::updateList()
{
    list_devices->clear();
    if(monitor_layout->currentIndex() == 0){ //单通道
        for(int j = 0; j < SettingInfo::cameras.size(); j++){
            if(SettingInfo::cameras[j].getChannel_id() == 1){
                QListWidgetItem* item_device = new QListWidgetItem(QIcon(":/images/device.png"),SettingInfo::cameras[j].getChannel_name());
                list_devices->addItem(item_device);
            }
        }
    }else{
        for(int i = 0; i < SettingInfo::cameras.size(); i++){
            for(int j = 0; j < SettingInfo::cameras.size(); j++){
                if(SettingInfo::cameras[j].getChannel_id() == i+1){
                    QListWidgetItem* item_device = new QListWidgetItem(QIcon(":/images/device.png"),SettingInfo::cameras[j].getChannel_name());
                    list_devices->addItem(item_device);
                }
            }

        }
    }
}

void MonitorWidget::switch_channel1()
{
    if(isSwitching) return;
    isSwitching = true;

    btn_w1->setEnabled(false);
    btn_w4->setEnabled(false);

    stopAllTasks();

    monitor_layout->setCurrentIndex(0);
    updateList();
    startMonitor(1);

    btn_w1->setEnabled(true);
    btn_w4->setEnabled(true);

    isSwitching = false;
}

void MonitorWidget::switch_channel4()
{
    if(isSwitching) return;
    isSwitching = true;

    btn_w1->setEnabled(false);
    btn_w4->setEnabled(false);

    stopAllTasks();

    monitor_layout->setCurrentIndex(1);
    updateList();
    startMonitor(4);

    btn_w1->setEnabled(true);
    btn_w4->setEnabled(true);

    isSwitching = false;
}

// 【优化】：UI 主线程专门的拉取与渲染逻辑，每间隔大约30FPS才会出发一次，处理共享变量中的frame
void MonitorWidget::renderFrames()
{
    cv::Mat frames_to_draw[4];
    {
        std::lock_guard<std::mutex> lock(this->frame_mutex);
        for(int i = 0; i < 4; i++){
            frames_to_draw[i] = this->latest_frames[i];
        }
    }

    if(this->monitor_layout->currentIndex() == 0){
        // 单通道模式
        if(!frames_to_draw[0].empty()){
            cv::Mat small_frame;
            QSize label_size = this->lab_monitor->size();

            // 【优化】：在转码前，先把矩阵缩小到 QLabel 的物理大小，从1080P大大缩小，极大减少像素量
            //使得转换时的计算量大大减少
            // 防止界面刚启动时 label 大小为 0
            if(label_size.width() > 10 && label_size.height() > 10){
                cv::resize(frames_to_draw[0], small_frame, cv::Size(label_size.width(), label_size.height()));
            }else{
                small_frame = frames_to_draw[0];
            }

            QPixmap pixmap = QPixmap::fromImage(MatToQImage(small_frame));
            this->lab_monitor->setPixmap(pixmap);
        }
    }else{
        //四通道模式
        for(int i = 0; i < 4; i++){
            if(!frames_to_draw[i].empty()){
                cv::Mat small_frame;
                QSize label_size = this->lab_monitors[i]->size();

                // 【优化】对 4 个通道分别进行缩小
                if(label_size.width() > 10 && label_size.height() > 10){
                    cv::resize(frames_to_draw[i], small_frame, cv::Size(label_size.width(), label_size.height()));
                }else{
                    small_frame = frames_to_draw[i];
                }

                QPixmap pixmap = QPixmap::fromImage(MatToQImage(small_frame));
                this->lab_monitors[i]->setPixmap(pixmap);
            }
        }
    }
}

void MonitorWidget::restartMonitor(int camera_count)
{
    current_camera_count = camera_count;
    stopAllTasks();
    startMonitor(camera_count);
}
