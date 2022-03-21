#ifndef PLAYLISTSVIEW_H
#define PLAYLISTSVIEW_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class PlaylistsView;
}

class PlaylistsView : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistsView(QWidget *parent = nullptr);
    ~PlaylistsView();

signals:
    void doubleClicked(QListWidgetItem *item);

public slots:
    void clearList();
    void addSong(QString item);
    void setSong(int index);
    int totalIndex();
    int currentSelection();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::PlaylistsView *ui;
};

#endif // PLAYLISTSVIEW_H
