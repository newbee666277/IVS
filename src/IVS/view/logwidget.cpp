#include "logwidget.h"

LogWidget::LogWidget(QWidget *parent)
    : QWidget{parent}
{
    //初始化页数变量
    this->cur_page = 1;
    this->page_size = 14;
    this->total_pages = 0;
    this->isNormal = true;

    //总体垂直布局
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    this->setLayout(main_layout);

    //上方整体横向布局
    QHBoxLayout* top_layout = new QHBoxLayout;
    //左边日志按钮
    QVBoxLayout* log_btn_layout = new QVBoxLayout;
    btn_normal_log = new QPushButton("操作日志");
    btn_exception_log = new QPushButton("异常日志");
    btn_normal_log->setObjectName("btn_type_log");
    btn_exception_log->setObjectName("btn_type_log");
    log_btn_layout->addWidget(btn_normal_log);
    log_btn_layout->addWidget(btn_exception_log);
    log_btn_layout->addStretch();
    //右边表格堆叠布局
    table_layout = new QStackedLayout;
    //操作日志表格
    table_normal_log = new QTableWidget(14,4);
    table_normal_log->setHorizontalHeaderLabels({"日志ID", "操作员", "操作功能", "操作时间"});
    table_normal_log->setSelectionBehavior(QAbstractItemView::SelectRows); // 点击时选中整行，而不是单个单元格
    table_normal_log->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 设置为只读，禁止用户双击修改日志
    table_normal_log->verticalHeader()->setVisible(false);                 // 隐藏左侧默认自带的丑陋序号(1,2,3...)
    table_normal_log->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 让4列自动拉伸填满整个表格宽度
    table_normal_log->setAlternatingRowColors(true);                       // 开启斑马线交替背景色
    table_normal_log->setShowGrid(false);                                  // 隐藏网格线，看起来更高级
    table_normal_log->setFocusPolicy(Qt::NoFocus);                         // 去除点击时单元格出现的虚线框
    // 1. 开启表格的鼠标追踪（原本必须按住鼠标滑动才追踪，现在纯滑过就会追踪）
    table_normal_log->setMouseTracking(true);

    // 2. 将我们的“魔法代理”装配给表格
    RowHoverDelegate* hoverDelegate = new RowHoverDelegate(table_normal_log);
    table_normal_log->setItemDelegate(hoverDelegate);

    // 3. 监听鼠标进入单元格的信号，更新行号并触发整个表格重绘
    connect(table_normal_log, &QTableWidget::cellEntered, this, [=](int row, int column){
        hoverDelegate->hoveredRow = row;
        table_normal_log->viewport()->update(); // 强制视图立刻刷新，展现整行高亮
    });

    //异常日志表格
    table_exception_log = new QTableWidget(14,5);
    table_exception_log->setHorizontalHeaderLabels({"日志ID", "操作员", "异常描述", "视频路径", "操作时间"});
    table_exception_log->setSelectionBehavior(QAbstractItemView::SelectRows); // 点击时选中整行，而不是单个单元格
    table_exception_log->setEditTriggers(QAbstractItemView::NoEditTriggers);  // 设置为只读，禁止用户双击修改日志
    table_exception_log->verticalHeader()->setVisible(false);                 // 隐藏左侧默认自带的丑陋序号(1,2,3...)
    table_exception_log->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 让4列自动拉伸填满整个表格宽度
    table_exception_log->setAlternatingRowColors(true);                       // 开启斑马线交替背景色
    table_exception_log->setShowGrid(false);                                  // 隐藏网格线，看起来更高级
    table_exception_log->setFocusPolicy(Qt::NoFocus);                         // 去除点击时单元格出现的虚线框
    // 1. 开启表格的鼠标追踪（原本必须按住鼠标滑动才追踪，现在纯滑过就会追踪）
    table_exception_log->setMouseTracking(true);

    // 2. 将我们的“魔法代理”装配给表格
    RowHoverDelegate* hoverDelegate_ex = new RowHoverDelegate(table_exception_log);
    table_exception_log->setItemDelegate(hoverDelegate_ex);

    // 3. 监听鼠标进入单元格的信号，更新行号并触发整个表格重绘
    connect(table_exception_log, &QTableWidget::cellEntered, this, [=](int row, int column){
        hoverDelegate_ex->hoveredRow = row;
        table_exception_log->viewport()->update(); // 强制视图立刻刷新，展现整行高亮
    });
    //table_normal_log->setObjectName("")
    //加入布局
    table_layout->addWidget(table_normal_log);
    table_layout->addWidget(table_exception_log);
    table_layout->setCurrentIndex(0);
    //加入上方布局
    top_layout->addLayout(log_btn_layout);
    top_layout->addLayout(table_layout);

    //下方横向布局
    QHBoxLayout* bottom_layout = new QHBoxLayout;
    btn_pre_page = new QPushButton("上一页");
    btn_next_page = new QPushButton("下一页");
    lab_page = new QLabel("1/1");
    lab_page->setObjectName("lab_page");
    bottom_layout->addStretch();
    bottom_layout->addWidget(btn_pre_page);
    bottom_layout->addWidget(lab_page);
    bottom_layout->addWidget(btn_next_page);
    bottom_layout->addStretch();

    main_layout->addLayout(top_layout, 8);
    main_layout->addLayout(bottom_layout, 1);

    this->init_qss();

    this->init_connect();

    this->updata_table();
}

void LogWidget::init_qss()
{
    QFile file("../qss/logwidget.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void LogWidget::init_connect()
{
    //操作日志按钮
    connect(btn_normal_log, &QPushButton::clicked, this, [this](){
        this->table_layout->setCurrentIndex(0);
        this->isNormal = true;
        updata_table();
    });

    //异常日志按钮
    connect(btn_exception_log, &QPushButton::clicked, this, [this](){
        this->table_layout->setCurrentIndex(1);
        this->isNormal = false;
        updata_table();
    });

    //翻页按钮
    connect(btn_next_page, &QPushButton::clicked, this, &LogWidget::next_page);
    connect(btn_pre_page, &QPushButton::clicked, this, &LogWidget::pre_page);
}

void LogWidget::updata_table()
{
    LogTask lt;
    this->total_pages = lt.query_total_pages(this->isNormal, this->page_size);
    QString lab_page;
    auto res = lt.fill_table(this->isNormal, lab_page, cur_page, total_pages, page_size);
    this->lab_page->setText(lab_page);
    if(isNormal){
        table_normal_log->clearContents();
        QList<NormalLogInfo> table_items = std::get<QList<NormalLogInfo>>(res);
        int row = 0;
        for(auto &item : table_items){
            table_normal_log->setItem(row, 0, new QTableWidgetItem(QString::number(item.getLog_id())));
            table_normal_log->setItem(row, 1, new QTableWidgetItem(item.getAdmin_name()));
            table_normal_log->setItem(row, 2, new QTableWidgetItem(item.getOperate_func()));
            table_normal_log->setItem(row, 3, new QTableWidgetItem(item.getOperate_time()));
            row++;
        }
    }else{
        table_exception_log->clearContents();
        QList<ExceptionLogInfo> table_items = std::get<QList<ExceptionLogInfo>>(res);
        int row = 0;
        for(auto &item : table_items){
            table_exception_log->setItem(row, 0, new QTableWidgetItem(QString::number(item.getLog_id())));
            table_exception_log->setItem(row, 1, new QTableWidgetItem(item.getAdmin_name()));
            table_exception_log->setItem(row, 2, new QTableWidgetItem(item.getException_desc()));
            table_exception_log->setItem(row, 3, new QTableWidgetItem(item.getVideo_path()));
            table_exception_log->setItem(row, 4, new QTableWidgetItem(item.getOperate_time()));
            row++;
        }
    }

}

void LogWidget::next_page()
{
    if(cur_page == total_pages){
        QMessageBox::warning(this, "警告", "已经是最后一页");
        return;
    }else{
        cur_page++;
        updata_table();
    }
}

void LogWidget::pre_page()
{
    if(cur_page == 1){
        QMessageBox::warning(this, "警告", "已经是第一页");
        return;
    }else{
        cur_page--;
        updata_table();
    }
}
