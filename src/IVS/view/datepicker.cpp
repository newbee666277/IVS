#include "datepicker.h"

DatePicker::DatePicker(QWidget *parent) : QWidget(parent)
{
    this->m_displayFormat = "yyyy-MM-dd";

    //初始化日期输入框
    // m_lineEdit = new QLineEdit(this);
    // m_lineEdit->setReadOnly(true); //设置为不可修改
    // m_lineEdit->setPlaceholderText("请输入日期"); //设置空白时的内容
    // m_lineEdit->setCursor(Qt::PointingHandCursor); //切换为鼠标手，更美观

    //初始化按钮
    QString cur_day = QDateTime::currentDateTime().toString(m_displayFormat);
    m_btn = new QPushButton(this);
    m_btn->setText(cur_day);

    //日历容器初始化
    m_calendarFram = new QFrame(nullptr, Qt::Tool | Qt::WindowStaysOnTopHint);
    m_calendarFram->setFrameShape(QFrame::StyledPanel); //设置样式
    m_calendarFram->setFrameShadow(QFrame::Raised); //设置立体阴影
    m_calendarFram->setVisible(false);

    //日期控件初始化
    m_calendar = new QCalendarWidget(m_calendarFram);
    m_calendar->setGridVisible(true);
    m_calendar->setMinimumDate(QDate(1900,1,1));
    m_calendar->setMaximumDate(QDate(2100, 12, 31));


    //设置默认日期
    setDate(QDate::currentDate());

    //布局初始化
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    //mainLayout->addWidget(m_lineEdit);
    mainLayout->addWidget(m_btn);
    mainLayout->setContentsMargins(0,0,0,0);
    QVBoxLayout* calendar_layout = new QVBoxLayout(m_calendarFram);
    calendar_layout->addWidget(m_calendar);
    calendar_layout->setContentsMargins(2,2,2,2);

    connect(m_btn, SIGNAL(clicked()), this, SLOT(toggleCalendarVisibility()));
    connect(m_calendar, SIGNAL(clicked(const QDate &)), this, SLOT(onDateSelected(const QDate &)));

    //m_lineEdit->installEventFilter(this);
}

void DatePicker::setDate(const QDate &date)
{
    if(date.isValid()){
        m_btn->setText(date.toString(this->m_displayFormat));
        m_calendar->setSelectedDate(date);
    }
}

QDate DatePicker::getDate() const
{
    return m_calendar->selectedDate();
}

void DatePicker::setDisplayFormat(const QString &format)
{
    this->m_displayFormat = format;
}

// bool DatePicker::eventFilter(QObject *obj, QEvent *event)
// {
//     if(obj == m_lineEdit && event->type() == QEvent::MouseButtonPress){
//         toggleCalendarVisibility();
//         return true;
//     }

//     return QObject::eventFilter(obj, event);
// }

void DatePicker::onDateSelected(const QDate &date)
{
    setDate(date);
    m_calendarFram->hide();
    emit dateSelected(date);
}

void DatePicker::toggleCalendarVisibility()
{
    if(m_calendar->isVisible()){
        m_calendarFram->hide();
    }else{
        //计算默认位置,输入框的正下方
        QPoint pos = mapToGlobal(m_btn->geometry().bottomLeft());

        m_calendarFram->adjustSize();
        QSize calendarSize = m_calendar->size();

        //屏幕边界检查，避免日历超出屏幕
        QRect screenRect = QGuiApplication::primaryScreen()->availableGeometry();
        //超出底部 显示在输入框上方
        if(pos.y() + calendarSize.height() > screenRect.bottom()){
            pos.setY(mapToGlobal(m_btn->geometry().topLeft()).y() - calendarSize.height());
        }

        //超出右侧，左移对齐
        if(pos.x() + calendarSize.width() > screenRect.right()){
            pos.setX(screenRect.right() - calendarSize.width());
        }

        //不超过屏幕左上角
        pos.setX(qMax(screenRect.left(), pos.x()));
        pos.setY(qMax(screenRect.top(), pos.y()));

        m_calendarFram->move(pos);
        m_calendarFram->show();
    }
}
