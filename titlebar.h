/*
 i took inspiration and code from this guy:
 https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle

 contribute to this since its kinda broken now
*/

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QMouseEvent>
#include <QWidget>

class titleBar : public QWidget
{
    Q_OBJECT
public:
    explicit titleBar(QWidget *parent = nullptr);
    virtual ~titleBar() {}

signals:
    void doubleClicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

protected:
    QPoint mousePos;
    QPoint wndPos;
    bool mousePressed;
};

#endif // TITLEBAR_H
