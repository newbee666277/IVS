#include "vcode.h"
#include <QPainter>
#include <QTime>
#include <QtGlobal>
#include <QMouseEvent>

VCode::VCode(QWidget *parent) : QWidget(parent), m_codeCount(4)
{
    this->setFixedSize(120, 40);

    // Qt 4.8 核心修改：使用当前时间作为随机数种子，否则每次运行生成的验证码都一样
    //qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 1000);

    generateRandCode();
}

void VCode::generateRandCode()
{
    const QString letters = "ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnpqrstuvwxyz23456789";
    m_code.clear();
    for (int i = 0; i < m_codeCount; ++i) {
        // 使用 randomInt() 取余数来获取随机索引
        int index = randomInt() % letters.length();
        m_code.append(letters.at(index));
    }
    update();
}

QString VCode::getCode() const
{
    return m_code;
}

QColor VCode::getRandColor()
{
    // 获取 0~179 之间的随机数，生成偏暗的颜色
    int r = randomInt() % 180;
    int g = randomInt() % 180;
    int b = randomInt() % 180;
    return QColor(r, g, b);
}

int VCode::randomInt() const
{
    static std::mt19937 engine(std::random_device{}());//0-32768之间
    std::uniform_int_distribution<int> dist(0,RAND_MAX);
    return dist(engine);
}

void VCode::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 填充背景
    painter.fillRect(this->rect(), QColor(240, 240, 240));

    // 绘制噪点
    for (int i = 0; i < 100; ++i) {
        painter.setPen(QPen(getRandColor(), 2));
        painter.drawPoint(randomInt() % width(), randomInt() % height());
    }

    // 绘制干扰线
    for (int i = 0; i < 6; ++i) {
        painter.setPen(QPen(getRandColor(), 1));
        painter.drawLine(randomInt() % width(), randomInt() % height(),
                         randomInt() % width(), randomInt() % height());
    }

    // 绘制文字
    QFont font("Arial", 20, QFont::Bold);
    painter.setFont(font);

    int charWidth = width() / m_codeCount;

    for (int i = 0; i < m_codeCount; ++i) {
        painter.setPen(getRandColor());

        // 随机位置偏移
        int xPos = i * charWidth + 5 + (randomInt() % 8);
        int yPos = height() / 2 + font.pointSize() / 2 - 5 + (randomInt() % 10);

        painter.drawText(xPos, yPos, QString(m_code.at(i)));
    }
}

void VCode::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    generateRandCode(); // 点击时刷新
}
