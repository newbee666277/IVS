#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QFile>
#include <QStackedLayout>
#include <QMessageBox>
#include <QStyledItemDelegate> // 引入样式代理类
#include <QPainter>            // 引入绘图类
#include "../service/logtask.h"

class LogWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton* btn_normal_log;
    QPushButton* btn_exception_log;
    QTableWidget* table_normal_log;
    QTableWidget* table_exception_log;
    QPushButton* btn_pre_page; //前一页
    QPushButton* btn_next_page; //下一页
    QLabel* lab_page; //显示页数
    QStackedLayout* table_layout;
    int cur_page;
    int page_size;
    int total_pages;
    bool isNormal; //判断当前是否为操作日志
public:
    explicit LogWidget(QWidget *parent = nullptr);
    void init_qss();
    void init_connect();
    void updata_table();

public slots:
    void next_page();
    void pre_page();
signals:
};

class RowHoverDelegate : public QStyledItemDelegate {
public:
    int hoveredRow = -1; // 记录当前鼠标在哪一行
    using QStyledItemDelegate::QStyledItemDelegate; // 继承构造函数

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QStyleOptionViewItem opt = option;

        // 核心魔法：如果当前正在绘制的单元格属于鼠标所在的行，强制给它打上“被悬浮”的状态戳
        if (index.row() == hoveredRow) {
            opt.state |= QStyle::State_MouseOver;
        } else {
            opt.state &= ~QStyle::State_MouseOver; // 其他行强制清除悬浮状态
        }

        QStyledItemDelegate::paint(painter, opt, index);
    }
};

#endif // LOGWIDGET_H
