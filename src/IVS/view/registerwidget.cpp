#include "registerwidget.h"
#include "../util/windowmanager.h"

RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowTitle("登录窗口");
    this->resize(1300, 700);

    //设置背景图片
    QPixmap pm(":/images/bg.jpg");
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(pm.scaled(this->size())));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    //设置窗口主布局为水平布局，方便最后将所有内容挤到右边，配合背景图片
    QHBoxLayout* main_layout = new QHBoxLayout(this);
    this->setLayout(main_layout);

    //主垂直布局
    QVBoxLayout* main_vlayout = new QVBoxLayout;

    QLabel* label_title = new QLabel("系统注册界面");
    label_title->setFont(QFont("宋体", 20, QFont::Bold));
    label_title->setStyleSheet("color: white;"); //字体颜色改为白色
    label_title->setAlignment(Qt::AlignCenter); //设置对齐方式为水平居中+垂直居中


    //表单布局
    QFormLayout* form_layout = new QFormLayout;
    //用户名即密码
    user_edit = new QLineEdit;
    pwd_edit = new QLineEdit;
    pwd_edit->setEchoMode(QLineEdit::Password);
    pwd_ensure_edit = new QLineEdit;
    pwd_ensure_edit->setEchoMode(QLineEdit::Password);
    QLabel* label_user = new QLabel("用户名");
    label_user->setStyleSheet("color: white;");
    QLabel* label_pwd = new QLabel("密  码");
    label_pwd->setStyleSheet("color: white;");
    QLabel* label_ensure_pwd = new QLabel("确认密码");
    label_ensure_pwd->setStyleSheet("color: white");
    form_layout->addRow(label_user, user_edit);
    form_layout->addRow(label_pwd, pwd_edit);
    form_layout->addRow(label_ensure_pwd, pwd_ensure_edit);
    form_layout->setSpacing(30);

    //原先表单布局的两个输入框拉的太长，将改布局加入水平布局中并在左右加两个弹簧将他们顶到中间
    QHBoxLayout* h_layout = new QHBoxLayout;
    h_layout->addStretch();
    h_layout->addLayout(form_layout);
    h_layout->addStretch();

    //按钮布局
    QHBoxLayout* btn_layout = new QHBoxLayout;
    reg_btn = new QPushButton("注册");
    return_log_btn = new QPushButton("取消");
    btn_layout->addWidget(reg_btn);
    btn_layout->addStretch(2);
    btn_layout->addWidget(return_log_btn);
    form_layout->addRow(btn_layout);

    //布局上中下加适当弹簧将布局顶美观
    main_vlayout->addStretch(1);
    main_vlayout->addWidget(label_title);
    main_vlayout->addStretch(1);
    main_vlayout->addLayout(h_layout);
    main_vlayout->addStretch(2);

    //总布局左边加弹簧将所有控件顶到右边，配合背景图片
    main_layout->addStretch();
    main_layout->addLayout(main_vlayout);

    this->init_connect();
}

void RegisterWidget::init_connect()
{
    //取消按钮，返回登录界面
    connect(return_log_btn, &QPushButton::clicked, this, [this](){
        WindowManager::getInstance()->loginwidget.show();
        this->hide();
    });

    //注册按钮
    connect(reg_btn, &QPushButton::clicked, this, [this](){
        UserSevice us;
        //先检查用户名是否重复
        QString cur_user_name = user_edit->text().trimmed();
        QString cur_pwd = pwd_edit->text().trimmed();
        QString cur_ensure_pwd = pwd_ensure_edit->text().trimmed();
        if(cur_user_name == "" || cur_pwd == "" || cur_ensure_pwd == ""){
            QMessageBox::warning(this, "警告", "用户名或密码不能为空");
            return;
        }

        if(cur_pwd == cur_ensure_pwd){ //先判断两次密码是否相同
            if(us.check_repeat_name(cur_user_name)){
                //重复
                QMessageBox::warning(this, "警告", "用户名已存在");
                this->user_edit->setText("");
                this->pwd_edit->setText("");
                this->pwd_ensure_edit->setText("");
                return;
            }else{
                //不重复
                if(us.reg(cur_user_name, cur_pwd)){
                    QMessageBox::information(this, "提示", "注册成功");
                    WindowManager::getInstance()->loginwidget.show();
                    this->hide();
                }else{
                    QMessageBox::warning(this, "警告", "注册失败, 数据库可能有问题");
                    this->user_edit->setText("");
                    this->pwd_edit->setText("");
                    this->pwd_ensure_edit->setText("");
                    return;
                }
            }
        }else{
            QMessageBox::warning(this, "警告", "两次密码不同");
            this->user_edit->setText("");
            this->pwd_edit->setText("");
            this->pwd_ensure_edit->setText("");
            return;
        }

    });
}
