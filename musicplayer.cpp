#include "musicplayer.h"
#include "ui_musicplayer.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    connect(mPlayer, &QMediaPlayer::playbackStateChanged, this, &MusicPlayer::checkPlaybackState);
    connect(mPlayer, &QMediaPlayer::mediaStatusChanged, this, &MusicPlayer::checkMediaState);
    connect(mPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::updatePositionSlider);
    connect(mPlayer, &QMediaPlayer::durationChanged, this, &MusicPlayer::updateDuration);
    connect(mPlayer, &QMediaPlayer::metaDataChanged, this , &MusicPlayer::updateInfo);
    qInfo() << appSettings.value("volumeLevel");
    showStartupMessage();
    ui->volumeSlider->setValue(appSettings.value("volumeLevel").toInt());
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

//TODO: remove some debug functions that arent needed anymore

//got this piece of code from qt's media player example

int getRand(int min, int max){
    unsigned int ms = static_cast<unsigned>(QDateTime::currentMSecsSinceEpoch());
    std::mt19937 gen(ms);
    std::uniform_int_distribution<> uid(min, max);
    return uid(gen);
}

static QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void MusicPlayer::showStartupMessage(){
    ui->statusbar->showMessage(startupMessages.at(getRand(0,5)));
}

void MusicPlayer::on_actionOpen_Song_triggered()
{
    QString soundPath = QFileDialog::getOpenFileName(this,tr("Open Music File"), "", tr("Music Files (*.mp3 *.wav *.flac)"));
    if (soundPath == ""){
        return;
    }
    else{
        fileName = QFileInfo(soundPath).fileName();
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
    if(mPlayer->playbackState() == QMediaPlayer::PlayingState){
        ui->progressSlider->setValue(mPlayer->position());
        ui->statusbar->showMessage(formatTime(mPlayer->position()) + " / " + formatTime(mPlayer->duration()));
    }
}

void MusicPlayer::updateDuration(){
    ui->progressSlider->setMaximum(mPlayer->duration());
}

void MusicPlayer::updateInfo()
{
    QStringList info;
    if (!fileName.isEmpty()){
        QString author = mPlayer->metaData().value(QMediaMetaData::AlbumArtist).toString();
        if (!author.isEmpty()){
            info.append(author);
        }
        QString title = mPlayer->metaData().value(QMediaMetaData::Title).toString();
        if (!title.isEmpty()){
            info.append(title);
        }
        QVariant songCover = mPlayer->metaData().value(QMediaMetaData::CoverArtImage);
        qInfo() << songCover;
        if (!songCover.isNull()) {
            QImage coverImage = songCover.value<QImage>();
            ui->coverImage->setPixmap(QPixmap::fromImage(coverImage));
        }
    }
    ui->songName->setText(info.join(tr(" - ")));
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
        ui->progressSlider->setValue(0);
    }
}


void MusicPlayer::on_actionOpen_Playlist_triggered()
{
    if (mPlayer->playbackState() == QMediaPlayer::PlayingState){
       mPlayer->stop();
    }

    QString filename = QFileDialog::getOpenFileName(this, "Open Playlist File...", "/", "JSON Files (*.json)");
    if(filename.isEmpty()){
        return;
    }
    else{
        QJsonDocument playlist;
        QJsonObject pl_entry;
        QJsonArray pl_entries;
        QByteArray json_data;
        QStringList song_list;
        QFile input(filename);
        if (input.open(QIODevice::ReadOnly | QIODevice::Text)){
            json_data = input.readAll();
            playlist = playlist.fromJson(json_data);
            pl_entries = playlist.array();
            qInfo() << pl_entries.count();
            model = new QStringListModel;
            for(int i=0; i< pl_entries.count(); i++){
                pl_entry = pl_entries.at(i).toObject();
                foreach (const QString& key, pl_entry.keys()){
                    QJsonValue value = pl_entry.value(key);
                    QString songEntry = value.toString();
                    song_list << songEntry;
                    ui->listWidget->addItem(songEntry);
                    qInfo() << value.toString();
                    songIndex = pl_entries.count() - pl_entries.count();
                    maxIndex = pl_entries.count();
                    qInfo() << songIndex;
                }

            }
            ui->listWidget->setCurrentRow(songIndex);
            mPlayer->setAudioOutput(outputDevice);
            mPlayer->setSource(QUrl::fromLocalFile(ui->listWidget->currentItem()->text()));
            isPlaylist = true;
        }
    }
}


void MusicPlayer::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
   QString selectedSong = item->text();
   qInfo() << selectedSong;
   mPlayer->setSource(QUrl::fromLocalFile(selectedSong));
   mPlayer->setAudioOutput(outputDevice);
   mPlayer->play();
}


void MusicPlayer::on_progressSlider_sliderPressed()
{
    if(mPlayer->playbackState() == QMediaPlayer::PlayingState){
        mPlayer->pause();
        outputDevice->setVolume(0);
    }
}


void MusicPlayer::on_progressSlider_sliderReleased()
{
    if(mPlayer->mediaStatus() != QMediaPlayer::NoMedia){
        if(mPlayer->playbackState() == QMediaPlayer::PausedState || mPlayer->playbackState() == QMediaPlayer::PlayingState ){
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            outputDevice->setVolume(floatVolume);
            mPlayer->play();
        }
        else if(mPlayer->playbackState() == QMediaPlayer::StoppedState && ui->progressSlider->value() != ui->progressSlider->maximum()){
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            outputDevice->setVolume(floatVolume);
            mPlayer->play();
        }
        else if(mPlayer->playbackState() == QMediaPlayer::StoppedState){
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            outputDevice->setVolume(floatVolume);
            mPlayer->stop();
        }
    }
}


void MusicPlayer::on_fowardButton_clicked()
{
    if(isPlaylist == true){
        if(songIndex <= ui->listWidget->model()->rowCount() - 2){
            songIndex++;
            qInfo() << songIndex;
            qInfo() <<  ui->listWidget->model()->rowCount() - 2;
            ui->listWidget->setCurrentRow(songIndex);
            QString selectedSong = ui->listWidget->currentItem()->text();
            mPlayer->setSource(QUrl::fromLocalFile(selectedSong));
            mPlayer->setAudioOutput(outputDevice);
            mPlayer->play();
        }
        else{
            return;
        }
    }
}


void MusicPlayer::on_rewindButton_clicked()
{
    if(isPlaylist == true){
        if(songIndex > 0){
            songIndex--;
            ui->listWidget->setCurrentRow(songIndex);
            QString selectedSong = ui->listWidget->currentItem()->text();
            mPlayer->setSource(QUrl::fromLocalFile(selectedSong));
            mPlayer->setAudioOutput(outputDevice);
            mPlayer->play();
        }
        else{
            return;
        }
    }
}


void MusicPlayer::on_progressSlider_sliderMoved(int position)
{
    if(mPlayer->playbackState() == QMediaPlayer::PausedState || mPlayer->playbackState() == QMediaPlayer::PlayingState){
        mPlayer->setPosition(position);
    }
}

void MusicPlayer::checkMediaState(){
    if(mPlayer->mediaStatus() == QMediaPlayer::EndOfMedia){
        if(isPlaylist == true){
            if(songIndex <= ui->listWidget->model()->rowCount() - 2){
                songIndex++;
                qInfo() << songIndex;
                qInfo() <<  ui->listWidget->model()->rowCount() - 2;
                ui->listWidget->setCurrentRow(songIndex);
                QString selectedSong = ui->listWidget->currentItem()->text();
                mPlayer->setSource(QUrl::fromLocalFile(selectedSong));
                mPlayer->setAudioOutput(outputDevice);
                mPlayer->play();
            }
            else{
                mPlayer->stop();
            }
        }
    }
}


void MusicPlayer::on_volumeSlider_sliderReleased()
{
    appSettings.setValue("volumeLevel", ui->volumeSlider->value());
    qInfo() << appSettings.value("volumeLevel");
}


void MusicPlayer::on_actionPlaylists_Editor_triggered()
{

}

