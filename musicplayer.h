#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudio>
#include <QtMultimedia/QtMultimedia>

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

    void updateInfo();

    void on_volumeSlider_sliderReleased();

private:
    Ui::MusicPlayer *ui;
    QMediaPlayer *mPlayer = new QMediaPlayer;
    QAudioOutput *outputDevice = new QAudioOutput;
    QStringListModel *model = new QStringListModel;
    bool isPlaylist;
    qint64 songIndex;
    qint64 maxIndex;
    QSettings appSettings;
    QString fileName;
    QStringList startupMessages = {"Teh Bezt Musik!", "What Are You Looking At?!?", "Cherryapple fans, rise up!", "ERROR: NO ERROR", "Long Live The FGP!", "Huh?!!?"};
};
#endif // MUSICPLAYER_H
