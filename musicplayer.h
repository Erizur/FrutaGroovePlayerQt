#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QLabel>
#include <QPushButton>
#include <QByteArray>
#include <QMessageBox>
#include <QStringListModel>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <aboutinfo.h>
#include <audiothread.h>
#include <windowhandler.h>
#include <playlistsview.h>
#include <random>
#include <QRandomGenerator>

#include <QSimpleUpdater.h>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/flacpicture.h>
#include <taglib/flacfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/mpegfile.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MusicPlayer; }
QT_END_NAMESPACE

class MusicPlayer : public QMainWindow
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();

private slots:
    void initializeButtonTheme();

    void on_actionOpen_Song_triggered();

    void on_playButton_clicked();

    void checkPlaybackState();

    void on_volumeSlider_valueChanged(int value);

    void on_stopButton_clicked();

    void updatePositionSlider();

    void updateDuration();

    void on_actionOpen_Playlist_triggered();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_progressSlider_sliderPressed();

    void on_progressSlider_sliderReleased();

    void on_fowardButton_clicked();

    void on_rewindButton_clicked();

    void on_progressSlider_sliderMoved(int position);

    void checkMediaState();

    void showStartupMessage();

    //QImage updateImage(QString path);

    void updateInfo();

    void on_volumeSlider_sliderReleased();

    void on_actionAbout_triggered();

    void handleError(int code);

    void on_actionLoop_triggered(bool checked);

    void on_actionShuffle_triggered(bool checked);

    void on_volumeSlider_sliderMoved(int position);

    void on_volumeButton_clicked();

    void on_actionPlaylists_triggered();

private:
    Ui::MusicPlayer *ui;
    AudioThread *player = new AudioThread;
    QStringListModel *model = new QStringListModel;
    QLabel *label = new QLabel("");
    QPushButton *shuffleBut = new QPushButton("");
    QPushButton *loopBut = new QPushButton("");
    bool isPlaylist;
    bool isLoop;
    bool isShuffle;
    bool isMod = false;
    qint64 songIndex;
    qint64 previousIndex;
    qint64 maxIndex;
    QStringList song_list;
    QString runtimeFile;
    QString fileName;
    QString currentTheme = "Light";
    QMessageBox messageBox;
    aboutInfo *abInf;
    PlaylistsView *playlistsView = new PlaylistsView();
    WindowHandler *plViewHandler = new WindowHandler();
    QStringList startupMessages = {"Teh Bezt Musik!", "What are you looking at?!?", "8+ year old music folder, rise up again!", "ERROR: JOKING AROUND", "Took me around an entire lifespan...", "Huh?!!?"};
};
#endif // MUSICPLAYER_H
