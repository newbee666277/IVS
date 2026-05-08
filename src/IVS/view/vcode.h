#ifndef VCODE_H
#define VCODE_H

#include <QWidget>
#include <QString>
#include <random>

class VCode : public QWidget
{
    Q_OBJECT
public:
    // Qt 4.8 默认可能不支持 nullptr，这里改回传统的 0
    explicit VCode(QWidget *parent = 0);
    QString getCode() const;
    void generateRandCode();
protected:
    // 去掉了 C++11 的 override 关键字
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:

    QColor getRandColor();
    int randomInt() const;
    QString m_code;
    int m_codeCount;
};

#endif // VCODE_H
