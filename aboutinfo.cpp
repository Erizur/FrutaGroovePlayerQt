#include "aboutinfo.h"
#include "ui_aboutinfo.h"

aboutInfo::aboutInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutInfo)
{
    ui->setupUi(this);
}

aboutInfo::~aboutInfo()
{
    delete ui;
}
