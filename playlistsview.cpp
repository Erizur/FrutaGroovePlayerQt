#include "playlistsview.h"
#include "ui_playlistsview.h"

PlaylistsView::PlaylistsView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistsView)
{
    ui->setupUi(this);
}

PlaylistsView::~PlaylistsView()
{
    delete ui;
}

void PlaylistsView::clearList()
{
    ui->listWidget->clear();
}

void PlaylistsView::addSong(QString item)
{
    ui->listWidget->addItem(item);
}

void PlaylistsView::setSong(int index)
{
    ui->listWidget->setCurrentRow(index);
}

int PlaylistsView::totalIndex()
{
    return ui->listWidget->model()->rowCount();
}

int PlaylistsView::currentSelection()
{
    return ui->listWidget->currentRow();
}
void PlaylistsView::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    emit doubleClicked(item);
}

