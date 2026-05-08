#include "videowidget.h"
#include "mainwidget.h"

VideoWidget::VideoWidget(QWidget *parent) : QWidget(parent)
{
    //初始化页数
    current_page = 1;
    page_size = 6;

    QHBoxLayout* main_layout = new QHBoxLayout;
    this->setLayout(main_layout);

    //回放窗口分为左右两个盒子
    left_widget = new QWidget;
    right_widget = new QWidget;
    main_layout->addWidget(left_widget, 2);
    main_layout->addWidget(right_widget, 7);

    this->init_left_widget();
    this->init_right_widget();

    this->init_qss();

    this->init_connect();


}

void VideoWidget::init_left_widget()
{
    QVBoxLayout* v_layout = new QVBoxLayout;
    this->left_widget->setLayout(v_layout);
    this->left_widget->setObjectName("left_box");
    VideoService vs;

    //顶部下拉框初始化
    QHBoxLayout* com_layout = new QHBoxLayout;
    //dates_com = new QComboBox;
    //日历控件选择日期
    datepicker = new DatePicker;
    channel_com = new QComboBox;
    channel_com->addItem("所有通道", 0);
    channel_com->addItem("通道1", 1);
    channel_com->addItem("通道2", 2);
    channel_com->addItem("通道3", 3);
    channel_com->addItem("通道4", 4);
    //init_dates_combo();
    com_layout->addWidget(datepicker);
    com_layout->addWidget(channel_com);

    //列表
    list_video = new QListWidget;
    list_video->setObjectName("days_list");
    lab_page = new QLabel; //底部页数显示
    // selected_date = dates_com->currentText();
    selected_date = datepicker->getDate().toString("yyyy-MM-dd");
    this->total_pages = vs.get_total_pages(selected_date, page_size, channel_com->currentData().toInt());
    update_list();
    //换页按钮布局
    QHBoxLayout *switch_page_layout = new QHBoxLayout;
    btn_pre = new QPushButton("上一页");
    btn_pre->setObjectName("btn_pre");
    btn_next = new QPushButton("下一页");
    btn_next->setObjectName("btn_next");

    lab_page->setAlignment(Qt::AlignCenter);

    switch_page_layout->addWidget(btn_pre);
    switch_page_layout->addWidget(lab_page);
    switch_page_layout->addWidget(btn_next);


    v_layout->addLayout(com_layout);
    v_layout->addWidget(list_video);
    v_layout->addLayout(switch_page_layout);
}

void VideoWidget::init_right_widget()
{
    //右盒子整体为垂直布局
    QVBoxLayout* global_right_layout = new QVBoxLayout;
    right_widget->setLayout(global_right_layout);

    //选择预览内容按钮
    QHBoxLayout* opt_layout = new QHBoxLayout;
    btn_video_opt = new QPushButton("查询视频");
    btn_video_opt->setObjectName("btn_video_opt");
    btn_img_opt = new QPushButton("查询图片");
    btn_img_opt->setObjectName("btn_img_opt");
    opt_layout->addStretch(1);
    opt_layout->addWidget(btn_video_opt);
    opt_layout->addStretch(2);
    opt_layout->addWidget(btn_img_opt);
    opt_layout->addStretch(1);
    global_right_layout->addLayout(opt_layout, 1);

    //下方堆叠布局
    right_stacked_layout = new QStackedLayout;
    global_right_layout->addLayout(right_stacked_layout, 9);
    QWidget* w_video = new QWidget;
    //视频组件整体为垂直布局
    video_layout = new QVBoxLayout;
    w_video->setLayout(video_layout);
    right_stacked_layout->addWidget(w_video);

    //上方视频显示
    //视频显示
    lab_video = new QLabel;
    lab_video->setPixmap(QPixmap(":/images/video_wrong.jpg"));
    lab_video->setScaledContents(true); //设置内容随窗口缩放
    lab_video->setAlignment(Qt::AlignCenter); //设置居中显示
    lab_video->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    video_layout->addWidget(lab_video, 8);

    //下方视频控制(进度条/播放暂停按钮/倍速/截图)
    //进度条
    video_slider = new QSlider(Qt::Horizontal);
    video_slider->setObjectName("video_slider");

    //控制按钮
    QHBoxLayout* video_ctr_layout = new QHBoxLayout;
    btn_ctr = new QPushButton();
    btn_ctr->setIcon(QIcon(":/images/pause.png"));
    btn_ctr->setIconSize(QSize(35,35));
    btn_ctr->setObjectName("btn_ctr");

    box_speed = new QComboBox;
    box_speed->setObjectName("box_speed");
    box_speed->addItem("0.5", 0.5);
    box_speed->addItem("1", 1);
    box_speed->addItem("2", 2);
    box_speed->addItem("3", 3);
    box_speed->setCurrentIndex(1); //默认显示1倍速

    btn_capture_img = new QPushButton("截图");
    btn_capture_img->setObjectName("btn_capture_img");

    video_ctr_layout->addStretch(2);
    video_ctr_layout->addWidget(btn_capture_img);
    video_ctr_layout->addStretch(1);
    video_ctr_layout->addWidget(btn_ctr);
    video_ctr_layout->addStretch(1);
    video_ctr_layout->addWidget(box_speed);
    video_ctr_layout->addStretch(2);

    video_layout->addWidget(video_slider);
    video_layout->addLayout(video_ctr_layout, 1);

    //索引1为图片查询界面
    QWidget* w_image = new QWidget;
    QVBoxLayout* img_layout = new QVBoxLayout;
    w_image->setLayout(img_layout);

    //上方的图片展示标签
    image_scroll_area = new QScrollArea;
    image_scroll_area->setWidgetResizable(true); // 允许内部自适应
    image_scroll_area->setAlignment(Qt::AlignCenter); // 图片居中显示
    image_scroll_area->setStyleSheet("QScrollArea { border: none; background-color: #222222; }");
    lab_image_display = new QLabel;
    lab_image_display->setAlignment(Qt::AlignCenter);
    lab_image_display->setText("请在左侧双击选择要查看的图片");
    lab_image_display->setStyleSheet("QLabel{ background-color: black; color: white; font-size: 20px; }");
    image_scroll_area->setWidget(lab_image_display);

    //下方图片操作和控制布局
    QHBoxLayout* img_ctr_layout = new QHBoxLayout;
    btn_related_video = new QPushButton("关联视频");
    btn_related_video->setObjectName("btn_related_video");
    btn_img_expand = new QPushButton("放大");
    btn_img_expand->setObjectName("btn_img_expand");
    slider_img_size = new QSlider(Qt::Horizontal);
    slider_img_size->setObjectName("slider_img_size");
    btn_img_reduce = new QPushButton("缩小");
    btn_img_reduce->setObjectName("btn_img_reduce");
    btn_img_fitwin = new QPushButton("恢复大小");
    btn_img_fitwin->setObjectName("btn_img_fitwin");
    img_ctr_layout->addWidget(btn_related_video);
    img_ctr_layout->addWidget(btn_img_expand);
    img_ctr_layout->addWidget(slider_img_size);
    img_ctr_layout->addWidget(btn_img_reduce);
    img_ctr_layout->addWidget(btn_img_fitwin);

    img_layout->addWidget(image_scroll_area, 8);
    img_layout->addLayout(img_ctr_layout, 1);
    right_stacked_layout->addWidget(w_image);
}

void VideoWidget::update_list()
{
    list_video->clear();
    VideoService vs;
    QString selected_date = datepicker->getDate().toString("yyyy-MM-dd");
    QString lab_page_str = "";
    int channel_id = channel_com->currentData().toInt();

    if(this->isVideoOpt){
        //当前为查询视频
        this->total_pages = vs.get_total_pages(selected_date, page_size, channel_id);
        if(current_page > total_pages) current_page = total_pages > 0 ? total_pages : 1;

        if(MainWidget::isNormalReview){
            QList<NormalVideoInfo> list_data;
            vs.update_list_normal(list_data, lab_page_str, selected_date, current_page, total_pages, page_size, channel_id);
            for(auto it = list_data.begin(); it != list_data.end(); it++){
                QListWidgetItem* item_day = new QListWidgetItem(QIcon(":/images/device.png"), it->getVideo_name());
                item_day->setData(Qt::UserRole, it->getVideo_path());
                list_video->addItem(item_day);
            }
        }else{
            QList<ExceptionVideoInfo> list_data;
            vs.update_list_exception(list_data, lab_page_str, selected_date, current_page, total_pages, page_size, channel_id);
            for(auto it = list_data.begin(); it != list_data.end(); it++){
                QListWidgetItem* item_day = new QListWidgetItem(QIcon(":/images/device.png"), it->getVideo_name());
                item_day->setData(Qt::UserRole, it->getRelated_video_path());
                list_video->addItem(item_day);
            }
        }
        this->lab_page->setText(lab_page_str);

    }else{
        //当前为查询图片
        // 0-手动截图(普通)，1-自动截图(异常)
        int feature_type = MainWidget::isNormalReview ? 0 : 1;
        FeatureModel fm;

        // 重新计算图片的总页数
        int total_records = fm.query_total_feature(selected_date, channel_id, feature_type);
        if(total_records < page_size){
            this->total_pages = 1;
        }else{
            this->total_pages = (total_records % page_size == 0) ? (total_records / page_size) : (total_records / page_size) + 1;
        }

        if(current_page > total_pages) current_page = total_pages > 0 ? total_pages : 1;
        lab_page_str = QString("%1/%2").arg(current_page).arg(total_pages);
        int offset = (current_page - 1) * page_size;

        // 获取图片数据
        QList<FeatureInfo> list_data;
        fm.fill_list_feature(list_data, selected_date, offset, page_size, channel_id, feature_type);

        // 更新左侧列表
        for(auto it = list_data.begin(); it != list_data.end(); it++){
            QListWidgetItem* item_day = new QListWidgetItem(QIcon(":/images/device.png"), it->getName());
            item_day->setData(Qt::UserRole, it->getPath());
            list_video->addItem(item_day);
        }
        this->lab_page->setText(lab_page_str);
    }
}

void VideoWidget::init_qss()
{
    QFile file(":/qss/videowidget.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void VideoWidget::init_connect()
{
    //翻页
    connect(btn_pre, SIGNAL(clicked()), this, SLOT(pre_page()));
    connect(btn_next, SIGNAL(clicked()), this, SLOT(next_page()));

    //切换日期
    connect(datepicker, &DatePicker::dateSelected, this, &VideoWidget::data_change);

    //切换通道
    connect(this->channel_com, &QComboBox::currentTextChanged, this, &VideoWidget::data_change);

    //点击通道名播放视频或者图片
    connect(this->list_video, &QListWidget::itemDoubleClicked, this, &VideoWidget::playVideo);

    //设置视频倍速
    connect(box_speed, &QComboBox::currentIndexChanged, this, [this](){

        if(v_task == nullptr){
            return;
        }

        v_task->setSpeed(box_speed->currentData().toDouble());
    });

    //视频播放与暂停
    connect(btn_ctr, &QPushButton::clicked, this, [this](){

        if(v_task == nullptr){
            QMessageBox::warning(this, "提示", "当前没有正在播放的视频");
            return;
        }

        if(!v_task->getIsPause()){
            v_task->pause();
            btn_ctr->setIcon(QIcon(":/images/continue.png"));
        }else{
            v_task->go();
            btn_ctr->setIcon(QIcon(":/images/pause.png"));
        }
    });

    //用户控制进度条
    connect(video_slider, &QSlider::sliderMoved, this, [this](int pos){
        if(v_task != nullptr){
            v_task->setPlayFrame(pos);
        }
    });

    //截图按钮
    connect(btn_capture_img, &QPushButton::clicked, this, [this](){
        FeatureService fs;
        QString file_path = "";
        QString name = "";
        if(fs.capture(this->img, cur_video_channel_id, file_path, name, false)){
            QMessageBox::information(this, "提示", "截图已保存至" + file_path);
            FeatureInfo f;
            FeatureModel fm;
            f.setName(name);
            f.setPath(file_path);
            f.setChannel_id(cur_video_channel_id);
            f.setException_id(0);
            f.setFeature_type(0);
            fm.insertFeatureInfo(f);
        }
        SettingWidget::logtask->recordLog(MainWidget::admin_id, MainWidget::admin_name, MainWidget::lab_time->text(), "截图");
    });

    //切换至查询视频
    connect(btn_video_opt, &QPushButton::clicked, this, [this](){
        this->isVideoOpt = true;
        this->right_stacked_layout->setCurrentIndex(0); // 切到视频界面
        this->data_change(); // 重置页码并刷新列表
    });

    //切换至查询图片
    connect(btn_img_opt, &QPushButton::clicked, this, [this](){
        this->isVideoOpt = false;
        this->stopCurVideoPlayThread(); // 停止后台还在播的视频
        this->right_stacked_layout->setCurrentIndex(1); // 切到图片界面
        this->lab_image_display->clear();
        this->lab_image_display->setText("请在左侧双击选择要查看的图片");
        this->data_change(); // 重置页码并刷新列表
    });

    //关联视频按钮
    connect(btn_related_video, &QPushButton::clicked, this, [this](){

        // 1. 调用 Model 获取视频路径
        FeatureModel fm;
        QString related_video_path = fm.query_video_path(this->path);

        // 2. 提前拦截：如果没有找到对应的视频，弹窗警告并立刻终止，绝不切换界面！
        if(related_video_path.isEmpty()){
            QMessageBox::warning(this, "提示", "未找到该截图关联的录像视频！（可能录像尚未生成或已被清理）");
            return;
        }

        // 3. 确定有视频后，更新路径并切换到视频界面
        this->path = related_video_path;
        this->isVideoOpt = true;
        this->right_stacked_layout->setCurrentIndex(0); // 切到视频展示面
        this->data_change(); // 重置左侧列表状态

        // 4. 解析通道号并启动播放线程
        cur_video_channel_id = path.at(path.size() - 5).digitValue();

        this->stopCurVideoPlayThread(); // 先停掉当前运行的视频播放线程
        v_task = new VideoPlayTask(path);
        v_cur_thread = new QThread;
        v_task->moveToThread(v_cur_thread);

        connect(v_cur_thread, &QThread::started, v_task, &VideoPlayTask::startPlay);
        connect(v_task, &VideoPlayTask::sendFrame, this, [this](cv::Mat frame, int cur_frame){
            //设置进度条
            this->video_slider->setMaximum(v_task->getFramecount());
            if(!this->video_slider->isSliderDown()){
                this->video_slider->setValue(cur_frame); //只有进度条没有被按下的时候才自动更新
            }

            this->img = MatToQImage(frame);
            this->update(); // 触发重绘
        });

        v_cur_thread->start();
    });

    //进度条变化
    connect(slider_img_size, &QSlider::valueChanged, this, [this](int value){
        double scale = value/50.0; //缩放倍数，基于100而言
        //基于base_image_pixmap进行缩放
        QPixmap scaledPixmap = this->base_image_pixmap.scaled(
            this->base_image_pixmap.size() * scale,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );

        this->lab_image_display->setPixmap(scaledPixmap);
    });

    //图片放大按钮
    connect(btn_img_expand, &QPushButton::clicked, this, [this](){
        slider_img_size->setValue(slider_img_size->value() + 10);
    });

    //图片缩小按钮
    connect(btn_img_reduce, &QPushButton::clicked, this, [this](){
        slider_img_size->setValue(slider_img_size->value() - 10);
    });

    //图片适应窗口按钮
    connect(btn_img_fitwin, &QPushButton::clicked, this, [this](){
        slider_img_size->setValue(50);
    });
}


void VideoWidget::pre_page()
{
    VideoService vs;
    if(current_page == 1){
        QMessageBox::information(this, "第一页警告", "已经是第一页");
        return;
    }else{
        current_page--;
        update_list();
    }
}

void VideoWidget::next_page()
{
    VideoService vs;
    if(current_page == total_pages){
        QMessageBox::information(this, "最后一页警告", "已经是最后一页");
        return;
    }else{
        current_page++;
        update_list();
    }
}

void VideoWidget::data_change()
{
    current_page = 1;
    selected_date = datepicker->getDate().toString("yyyy-MM-dd");
    update_list();
}

void VideoWidget::playVideo(QListWidgetItem *item)
{
    //qDebug() << "当前点击的视频路径为: " <<  item->data(Qt::UserRole).toString();
    path = item->data(Qt::UserRole).toString();

    if(path.isEmpty()){
        QMessageBox::warning(this, "警告", "文件路径无效！");
        return;
    }

    if(this->isVideoOpt){
        cur_video_channel_id = path.at(path.size() - 5).digitValue();
        //qDebug() << cur_video_channel_id;

        this->stopCurVideoPlayThread(); //先停掉当前运行的视频播放线程
        v_task = new VideoPlayTask(path);
        v_cur_thread = new QThread;
        v_task->moveToThread(v_cur_thread);


        connect(v_cur_thread, &QThread::started, v_task, &VideoPlayTask::startPlay);
        VideoPlayTask* task = v_task;

        connect(task, &VideoPlayTask::sendFrame, this, [this, task](cv::Mat frame, int cur_frame){

            // 如果当前任务已经不是原来的 task，说明已经切换到图片或换了视频
            if(task != this->v_task || this->v_task == nullptr){
                return;
            }

            this->video_slider->setMaximum(task->getFramecount());

            if(!this->video_slider->isSliderDown()){
                this->video_slider->setValue(cur_frame);
            }

            this->img = MatToQImage(frame);

            // 只有当前仍然是视频界面时才刷新视频画面
            if(this->isVideoOpt){
                this->update();
            }
        });
        v_cur_thread->start();

    }else{
        QPixmap pixmap(path);
        if(pixmap.isNull()){
            QMessageBox::warning(this, "错误", "无法打开该图片，文件可能已丢失或损坏: " + path);
            return;
        }

        QSize areasize = image_scroll_area->viewport()->size(); //当前区域大小
        if(areasize.width() < 100){
            areasize = QSize(800, 600);
        }
        base_image_pixmap = pixmap.scaled(areasize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        this->lab_image_display->setPixmap(base_image_pixmap);

        //进度条设置,初始默认为中间位置
        slider_img_size->blockSignals(true);
        slider_img_size->setValue(50);
        slider_img_size->blockSignals(false);
    }
}


QImage VideoWidget::MatToQImage(Mat& frame)
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

void VideoWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if(!isVideoOpt){
        return;
    }

    if(img.isNull()){
        return;
    }

    if(lab_video == nullptr || lab_video->width() <= 0){
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(this->img);
    this->lab_video->setPixmap(
        pixmap.scaledToWidth(this->lab_video->width(), Qt::SmoothTransformation)
        );
}

void VideoWidget::stopCurVideoPlayThread()
{
    if(v_cur_thread == nullptr){
        return;
    }

    VideoPlayTask* oldTask = v_task;
    QThread* oldThread = v_cur_thread;

    // 先把成员变量置空，防止残留信号继续使用
    v_task = nullptr;
    v_cur_thread = nullptr;

    if(oldTask != nullptr){
        disconnect(oldTask, nullptr, this, nullptr);
        oldTask->stop();
    }

    if(oldThread->isRunning()){
        oldThread->quit();
        oldThread->wait();
    }

    delete oldTask;
    delete oldThread;
}



