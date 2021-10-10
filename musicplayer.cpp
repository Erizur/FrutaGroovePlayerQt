#include "musicplayer.h"
#include "ui_musicplayer.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    connect(mPlayer, &QMediaPlayer::playbackStateChanged, this, &MusicPlayer::checkPlaybackState);
    connect(mPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::updatePositionSlider);
    connect(mPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateDuration);
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}


void MusicPlayer::on_actionOpen_Song_triggered()
{
    QString soundPath = QFileDialog::getOpenFileName(this,tr("Open Music File"), "", tr("Music Files (*.mp3 *.wav *.flac)"));
    if (soundPath == ""){
        return;
    }
    else{
        mPlayer->setAudioOutput(outputDevice);
        mPlayer->setSource(QUrl::fromLocalFile(soundPath));
    }
}

void MusicPlayer::checkPlaybackState(){
    if(mPlayer->playbackState() == QMediaPlayer::PlayingState){
        ui->playButton->setText("Pause");
    }
    else if(mPlayer->playbackState() == QMediaPlayer::StoppedState || mPlayer->playbackState() == QMediaPlayer::PausedState){
        ui->playButton->setText("Play");
    }
}

void MusicPlayer::on_playButton_clicked()
{
    if(mPlayer->mediaStatus() == QMediaPlayer::NoMedia){
        return;
    }
    else if(mPlayer->mediaStatus() == QMediaPlayer::EndOfMedia){
        if(mPlayer->playbackState() == QMediaPlayer::StoppedState || mPlayer->playbackState() == QMediaPlayer::PausedState){
            ui->playButton->setText("Pause");
            mPlayer->play();
        }
    }
    else{
        if(mPlayer->playbackState() == QMediaPlayer::StoppedState || mPlayer->playbackState() == QMediaPlayer::PausedState){
            ui->playButton->setText("Pause");
            mPlayer->play();
        }
        else{
            mPlayer->pause();
        }
    }
}

void MusicPlayer::updatePositionSlider(){
    ui->progressSlider->setValue(mPlayer->position());
}

void MusicPlayer::updateDuration(){
    ui->progressSlider->setMaximum(mPlayer->duration());
}

void MusicPlayer::on_volumeSlider_valueChanged(int value)
{
    float floatVolume = (float)value / 100;
    outputDevice->setVolume(floatVolume);
}

void MusicPlayer::on_stopButton_clicked()
{
    if(mPlayer->playbackState() == QMediaPlayer::PlayingState || mPlayer->playbackState() == QMediaPlayer::PausedState){
        mPlayer->stop();
    }
}

