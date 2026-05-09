#include "loginwidget.h"
#include "../util/windowmanager.h"

loginWidget::loginWidget(QWidget *parent) : QWidget(parent)
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

    QLabel* label_title = new QLabel("系统登录界面");
    label_title->setFont(QFont("宋体", 20, QFont::Bold));
    label_title->setStyleSheet("color: white;"); //字体颜色改为白色
    label_title->setAlignment(Qt::AlignCenter); //设置对齐方式为水平居中+垂直居中


    //表单布局
    QFormLayout* form_layout = new QFormLayout;
    //用户名即密码
    user_edit = new QLineEdit;
    pwd_edit = new QLineEdit;
    pwd_edit->setEchoMode(QLineEdit::Password);
    QLabel* label_user = new QLabel("用户名");
    label_user->setStyleSheet("color: white;");
    QLabel* label_pwd = new QLabel("密  码");
    label_pwd->setStyleSheet("color: white;");
    form_layout->addRow(label_user, user_edit);
    form_layout->addRow(label_pwd, pwd_edit);
    form_layout->setSpacing(30);

    //原先表单布局的两个输入框拉的太长，将改布局加入水平布局中并在左右加两个弹簧将他们顶到中间
    QHBoxLayout* h_layout = new QHBoxLayout;
    h_layout->addStretch();
    h_layout->addLayout(form_layout);
    h_layout->addStretch();

    //验证码布局
    QHBoxLayout* vcode_layout = new QHBoxLayout;
    QLabel* label_vcode = new QLabel("验证码");
    label_vcode->setStyleSheet("color: white;");
    //调整输入框大小
    edit_vcode = new QLineEdit;
    edit_vcode->setMinimumWidth(40);
    edit_vcode->setMaximumWidth(210);
    edit_vcode->setMaxLength(4);
    vcode = new VCode;
    vcode_layout->addWidget(label_vcode);
    vcode_layout->addWidget(edit_vcode);
    vcode_layout->addWidget(vcode);
    form_layout->addRow(vcode_layout);

    //按钮布局
    QHBoxLayout* btn_layout = new QHBoxLayout;
    log_btn = new QPushButton("登录");
    reg_btn = new QPushButton("注册");
    cancel_btn = new QPushButton("取消");
    btn_layout->addWidget(log_btn);
    btn_layout->addWidget(reg_btn);
    btn_layout->addWidget(cancel_btn);
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

    init_connect();
}

void loginWidget::init_connect()
{
    //登录按钮
    connect(log_btn, SIGNAL(clicked()), this, SLOT(login()));

    //注册按钮
    connect(reg_btn, &QPushButton::clicked, this, &loginWidget::reg);

    //取消按钮
    connect(cancel_btn, &QPushButton::clicked, this, [this](){
        WindowManager::getInstance()->mainwidget.show();
        this->hide();
    });

}

void loginWidget::login()
{
    QString username = user_edit->text().trimmed();
    QString password = pwd_edit->text().trimmed();

    //先检查验证码
    QString vcode_str = edit_vcode->text().trimmed();
    if(QString::compare(vcode_str,this->vcode->getCode(),Qt::CaseInsensitive) != 0){
        QMessageBox::warning(this, "登录失败", "验证码错误");
        this->user_edit->setText("");
        this->pwd_edit->setText("");
        this->edit_vcode->setText("");
        this->vcode->generateRandCode();
        return;
    }

    UserSevice us;
    int res = us.login(username, password);

    if(res){
        //emit login_success(username);
        MainWidget::admin_name = username; //记录全局管理员名称
        MainWidget::admin_id = res;
        WindowManager::getInstance()->mainwidget.showWidget(username);
        this->hide();
    }else{
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
        this->user_edit->setText("");
        this->pwd_edit->setText("");
        this->edit_vcode->setText("");
        this->vcode->generateRandCode();
    }


}

void loginWidget::reg()
{
    WindowManager::getInstance()->registerwidget.show();
    this->hide();
}
