#include "musicplayer.h"
#include "ui_musicplayer.h"
#include "audiothread.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    connect(player, &AudioThread::positionChanged, this, &MusicPlayer::updatePositionSlider);
    connect(mPlayer, &QMediaPlayer::mediaStatusChanged, this, &MusicPlayer::checkMediaState);
    connect(player, &AudioThread::endOfPlayback, this, &MusicPlayer::checkMediaState);
    connect(player, &AudioThread::stateChanged, this, &MusicPlayer::checkPlaybackState);
    //connect(mPlayer, &QMediaPlayer::positionChanged, this, &MusicPlayer::updatePositionSlider);
    connect(player, &AudioThread::lengthChanged, this, &MusicPlayer::updateDuration);
    //connect(mPlayer, &QMediaPlayer::metaDataChanged, this , &MusicPlayer::updateInfo);
    QSettings appSettings("AM_Erizur", "FrutaGroovePlayer");
    if(!appSettings.value("volumeLevel").isValid()){
        ui->volumeSlider->setValue(100);
    }
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
    ui->statusbar->showMessage(startupMessages.at(getRand(0,5)), 5000);
}

void MusicPlayer::on_actionOpen_Song_triggered()
{
    if(player->loaded == true){
        player->stop();
    }
    QString soundPath = QFileDialog::getOpenFileName(this,tr("Open Audio File"), "", tr("Audio Files (*.mp3 *.wav *.flac *.ogg)"));
    if(isPlaylist == true){
        isPlaylist = false;
        ui->listWidget->clear();
    }
    if (soundPath == ""){
        return;
    }
    else{
        fileName = QFileInfo(soundPath).filePath();
        player->playing = false;
    }
}

void MusicPlayer::checkPlaybackState(){
    if(player->playing == true){
        ui->playButton->setText("Pause");
    }
    else{
        ui->playButton->setText("Play");
    }
}

void MusicPlayer::on_playButton_clicked()
{
    if (fileName != ""){
       if(player->playing == false){
           if(player->loaded == true){
               player->resume();
           }
           else{
               player->play(fileName);
           }
           float floatVolume = (float)ui->volumeSlider->value() / 100;
           player->changeVolume(floatVolume);
           ui->playButton->setText("Pause");
       }
       else{
           player->pause();
           ui->playButton->setText("Play");
       }

    }
}

void MusicPlayer::updatePositionSlider(){
    ui->progressSlider->setValue(player->currentPosition());
    ui->statusbar->showMessage(formatTime(player->currentPosSeconds()) + " / " + formatTime(player->audioLengthSeconds()));
}

void MusicPlayer::updateDuration(){
    ui->progressSlider->setMaximum(player->audioLength());
}

//void MusicPlayer::updateInfo()
//{
//    QStringList info;
//    if (!fileName.isEmpty()){
//        TagLib::FileRef f(fileName.toStdString().c_str());
//        TagLib::String artist_string = f.tag()->artist();
//        TagLib::String title_string = f.tag()->title();
//        QString author = QString::fromStdWString(artist_string.toWString());
//        if (!author.isEmpty()){
//            info.append(author);
//        }
//        QString title = QString::fromStdWString(title_string.toWString());
//        if (!title.isEmpty()){
//            info.append(title);
//        }
//        QVariant songCover = mPlayer->metaData().value(QMediaMetaData::CoverArtImage);
//        qInfo() << songCover;
//        if (songCover.isValid()) {
//            QImage coverImage = songCover.value<QImage>();
//            ui->coverImage->setPixmap(QPixmap::fromImage(coverImage));
//        }
//    }
//    ui->songName->setText(info.join(tr(" - ")));
//}


void MusicPlayer::on_volumeSlider_valueChanged(int value)
{
    float floatVolume = (float)value / 100;
    player->changeVolume(floatVolume);
}

void MusicPlayer::on_stopButton_clicked()
{
    player->stop();
}


void MusicPlayer::on_actionOpen_Playlist_triggered()
{
    if(isPlaylist == true){
        ui->listWidget->clear();
        for(int idx = 0; idx < song_list.size(); ++idx)
        {
            song_list.remove(idx);
            --idx;
        }
    }
    if(player->loaded == true){
        player->stop();
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
                    QFileInfo fileThingie(songEntry);
                    song_list << songEntry;
                    ui->listWidget->addItem(fileThingie.baseName());
                    qInfo() << value.toString();
                    songIndex = pl_entries.count() - pl_entries.count();
                    maxIndex = pl_entries.count();
                    qInfo() << songIndex;
                }

            }
            ui->listWidget->setCurrentRow(songIndex);
            fileName = song_list.at(songIndex).toLocal8Bit().constData();
            isPlaylist = true;
            player->playing = false;
        }
    }
}


void MusicPlayer::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
   songIndex = ui->listWidget->row(item);
   if(player->loaded == true){
       player->stop();
   }
   fileName = song_list.at(songIndex).toLocal8Bit().constData();
   player->play(fileName);
   float floatVolume = (float)ui->volumeSlider->value() / 100;
   player->changeVolume(floatVolume);
}


void MusicPlayer::on_progressSlider_sliderPressed()
{
    if(player->loaded == true){
        player->pause();
    }
}


void MusicPlayer::on_progressSlider_sliderReleased()
{
    if(player->loaded == true){
        player->resume();
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
            if(player->loaded == true){
                player->stop();
            }
            fileName = song_list.at(songIndex).toLocal8Bit().constData();
            player->play(fileName);
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            player->changeVolume(floatVolume);
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
            if(player->loaded == true){
                player->stop();
            }
            fileName = song_list.at(songIndex).toLocal8Bit().constData();
            player->play(fileName);
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            player->changeVolume(floatVolume);
        }
        else{
            return;
        }
    }
}


void MusicPlayer::on_progressSlider_sliderMoved(int position)
{
    qInfo() << position;
    player->changePosition(position);
}

void MusicPlayer::checkMediaState(){
//    if(mPlayer->mediaStatus() == QMediaPlayer::EndOfMedia){
//        if(isPlaylist == true){
//            if(songIndex <= ui->listWidget->model()->rowCount() - 2){
//                songIndex++;
//                qInfo() << songIndex;
//                qInfo() <<  ui->listWidget->model()->rowCount() - 2;
//                ui->listWidget->setCurrentRow(songIndex);
//                QString selectedSong = ui->listWidget->currentItem()->text();
//                mPlayer->setSource(QUrl::fromLocalFile(selectedSong));
//                mPlayer->setAudioOutput(outputDevice);
//                mPlayer->play();
//            }
//            else{
//                mPlayer->stop();
//            }
//        }
//    }
    if(isPlaylist == true){
        if(songIndex <= ui->listWidget->model()->rowCount() - 2){
            songIndex++;
            qInfo() << songIndex;
            qInfo() <<  ui->listWidget->model()->rowCount() - 2;
            ui->listWidget->setCurrentRow(songIndex);
            if(player->loaded == true){
                player->stop();
            }
            fileName = song_list.at(songIndex).toLocal8Bit().constData();
            player->play(fileName);
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            player->changeVolume(floatVolume);
        }
        else{
            return;
        }
    }
    else{
        player->stop();
    }
}


void MusicPlayer::on_volumeSlider_sliderReleased()
{
    QSettings appSettings("AM_Erizur", "FrutaGroovePlayer");
    appSettings.setValue("volumeLevel", ui->volumeSlider->value());
    qInfo() << appSettings.value("volumeLevel");
}


void MusicPlayer::on_actionPlaylists_Editor_triggered()
{

}


void MusicPlayer::on_actionAbout_triggered()
{
    abInf = new aboutInfo(); // Be sure to destroy your window somewhere
    abInf->show();
}

