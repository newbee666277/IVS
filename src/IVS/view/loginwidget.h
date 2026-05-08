#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QPixmap>
#include "vcode.h"
#include "../util/dbconn.h"
#include "../service/usersevice.h"
#include <QMessageBox>

class loginWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton* log_btn;
    QLineEdit* user_edit;
    QLineEdit* pwd_edit;
    QLineEdit* edit_vcode;
    QPushButton* reg_btn;
    QPushButton* cancel_btn;
    VCode* vcode;
public:
    explicit loginWidget(QWidget *parent = nullptr);
    void init_connect();
signals:
    void login_success(QString);

public slots:
    void login();
    void reg();
};



#endif // LOGINWIDGET_H
