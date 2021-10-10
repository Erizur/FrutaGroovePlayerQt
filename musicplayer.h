#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMainWindow>
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

private:
    Ui::MusicPlayer *ui;
    QMediaPlayer *mPlayer = new QMediaPlayer;
    QAudioOutput *outputDevice = new QAudioOutput;
};
#endif // MUSICPLAYER_H
