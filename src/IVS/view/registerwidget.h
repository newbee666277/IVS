#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include "../service/usersevice.h"

class RegisterWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton* reg_btn;
    QPushButton* cancel_btn;
    QPushButton* return_log_btn;//返回登录界面
    QLineEdit* user_edit;
    QLineEdit* pwd_edit;
    QLineEdit* pwd_ensure_edit;
public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    void init_connect();
signals:
};

#endif // REGISTERWIDGET_H
