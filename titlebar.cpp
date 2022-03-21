#include "titlebar.h"
#include <QPainter>
#include <QStyleOption>

titleBar::titleBar(QWidget *parent) : QWidget{parent}
{
    mousePressed = false;
}

void titleBar::mousePressEvent(QMouseEvent *event){
    mousePressed = true;
    mousePos = event->globalPos();

    QWidget *parent = parentWidget();
    if (parent) parent = parent->parentWidget();

    if (parent) wndPos = parent->pos();
}

void titleBar::mouseMoveEvent(QMouseEvent *event) {
    QWidget *parent = parentWidget();
    if (parent) parent = parent->parentWidget();

    if (parent && mousePressed)
        parent->move(wndPos + (event->globalPos() - mousePos));
}

void titleBar::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    mousePressed = false;
}

void titleBar::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QStyleOption styleOption;
    styleOption.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
}

void titleBar::mouseDoubleClickEvent(QMouseEvent *event){
    Q_UNUSED(event);
    emit doubleClicked();
}
