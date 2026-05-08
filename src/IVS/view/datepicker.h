#ifndef DATEPICKER_H
#define DATEPICKER_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QDate>
#include <QDebug>
#include <QScreen>
#include <QEvent>
#include <QCalendarWidget>
#include <QString>
#include <QGuiApplication>

class DatePicker : public QWidget
{
    Q_OBJECT
private:
    QLineEdit* m_lineEdit; //日期显示输入框
    QPushButton* m_btn; //切换日历显示和隐藏的按钮
    QFrame* m_calendarFram; //日历的容器控件
    QCalendarWidget* m_calendar; //日历控件
    QString m_displayFormat; //日期显示格式
public:
    explicit DatePicker(QWidget *parent = nullptr);
    void setDate(const QDate &date); //设置日期
    QDate getDate() const; //获取选中的日期
    void setDisplayFormat(const QString &format); //设置日期显示格式
signals:
    void dateSelected(const QDate &date);
protected:
    //bool eventFilter(QObject* obj, QEvent* event); //事件过滤器
public slots:
    void onDateSelected(const QDate &date); //日历控件选中日期
    void toggleCalendarVisibility(); //切换日历的显示与隐藏
};

#endif // DATEPICKER_H