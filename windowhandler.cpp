#include "windowhandler.h"
#include "ui_windowhandler.h"
#include <QApplication>
#include <QGraphicsDropShadowEffect>

WindowHandler::WindowHandler(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowHandler)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    #if defined(Q_OS_WIN)
        setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    #endif
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_QuitOnClose, false); //by default, can be enabled again

    QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
    windowShadow->setBlurRadius(12.0);
    windowShadow->setColor(palette().color(QPalette::Shadow));
    windowShadow->setOffset(0.0);
    ui->winFrame->setGraphicsEffect(windowShadow);

    QObject::connect(qApp, &QGuiApplication::applicationStateChanged, this, &WindowHandler::appStateChanged);
    setMouseTracking(true);
    QApplication::instance()->installEventFilter(this);

}

WindowHandler::~WindowHandler()
{
    delete ui;
}

void WindowHandler::setContent(QWidget *w) {
  ui->winContent->layout()->addWidget(w);
}

void WindowHandler::setSize(QSize size){
    ui->winFrame->setFixedWidth(size.width());
    ui->winFrame->setFixedHeight(size.height() + ui->winTitlebar->size().height());
}

void WindowHandler::on_closeButton_clicked()
{
    close();
}

void WindowHandler::setTitle(const QString &text) {
  ui->titleText->setText(text);
  this->setWindowTitle(text);
}

void WindowHandler::setIcon(const QIcon &ico) {
  ui->icon->setPixmap(ico.pixmap(18, 18));
  this->setWindowIcon(ico);
}

void WindowHandler::on_minimizeButton_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

void WindowHandler::appStateChanged(Qt::ApplicationState state)
{
    if (state == Qt::ApplicationActive) {
        ui->titleText->setProperty("active", true);
        ui->winTitlebar->setProperty("active", true);
        ui->winFrame->setProperty("active", true);
        ui->titleText->setProperty("inactive", false);
        ui->winTitlebar->setProperty("inactive", false);
        ui->winFrame->setProperty("inactive", false);
    } else {
        ui->titleText->setProperty("active", false);
        ui->winTitlebar->setProperty("active", false);
        ui->winFrame->setProperty("active", false);
        ui->titleText->setProperty("inactive", true);
        ui->winTitlebar->setProperty("inactive", true);
        ui->winFrame->setProperty("inactive", true);
    }
}


