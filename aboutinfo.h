#ifndef ABOUTINFO_H
#define ABOUTINFO_H

#include <QDialog>

namespace Ui {
class aboutInfo;
}

class aboutInfo : public QDialog
{
    Q_OBJECT

public:
    explicit aboutInfo(QWidget *parent = nullptr);
    ~aboutInfo();

private:
    Ui::aboutInfo *ui;
};

#endif // ABOUTINFO_H
