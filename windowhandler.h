#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include <QWidget>
#include <QColor>

namespace Ui {
class WindowHandler;
}

class WindowHandler : public QWidget
{
    Q_OBJECT

public:
    explicit WindowHandler(QWidget *parent = nullptr);
    ~WindowHandler();

public slots:
    void setContent(QWidget *w);

    void setSize(QSize size);

    void on_closeButton_clicked();

    void on_minimizeButton_clicked();

    void setTitle(const QString &text);

    void setIcon(const QIcon &ico);

    void appStateChanged(Qt::ApplicationState state);

private:
    Ui::WindowHandler *ui;
};

#endif // WINDOWHANDLER_H
