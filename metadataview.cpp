#include "metadataview.h"
#include "ui_metadataview.h"

#include <QFile>
#include <QFileInfo>
#include <QUrl>

MetadataView::MetadataView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MetadataView)
{
    ui->setupUi(this);
}

MetadataView::~MetadataView()
{
    delete ui;
}

void MetadataView::setCoverArt(QImage image)
{
    if(!image.isNull()){
        ui->albumCover->setPixmap(QPixmap::fromImage(image));
        qInfo() << "theres a cover file";
    }
    else{
        QPixmap img(":/images/resources/noCover.png");
        ui->albumCover->setPixmap(img);
        qInfo() << "no cover";
    }
}
