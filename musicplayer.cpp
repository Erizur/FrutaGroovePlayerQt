#include "musicplayer.h"
#include "ui_musicplayer.h"
#include "audiothread.h"

MusicPlayer::MusicPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    connect(player, &AudioThread::positionChanged, this, &MusicPlayer::updatePositionSlider);
    connect(player, &AudioThread::endOfPlayback, this, &MusicPlayer::checkMediaState);
    connect(player, &AudioThread::stateChanged, this, &MusicPlayer::checkPlaybackState);
    connect(player, &AudioThread::lengthChanged, this, &MusicPlayer::updateDuration);
    connect(player, &AudioThread::metadataChanged, this, &MusicPlayer::updateInfo);
    connect(player, SIGNAL(errorHappened(int)), this, SLOT(handleError(int)));
    connect(playlistsView, SIGNAL(doubleClicked(QListWidgetItem*)), this, SLOT(on_listWidget_itemDoubleClicked(QListWidgetItem*)));
    connect(shuffleBut, SIGNAL(toggled(bool)), this, SLOT(on_actionShuffle_triggered(bool)));
    connect(loopBut, SIGNAL(toggled(bool)), this, SLOT(on_actionLoop_triggered(bool)));
    QSettings appSettings("AM_Erizur", "FrutaGroovePlayer");
    if(!appSettings.value("volumeLevel").isValid()){
        ui->volumeSlider->setValue(100);
    }
    qInfo() << appSettings.value("volumeLevel");
    shuffleBut->setIcon(QIcon(":/images/resources/buttonShuffle" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
    loopBut->setIcon(QIcon(":/images/resources/buttonLoop" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
    shuffleBut->setCheckable(true);
    loopBut->setCheckable(true);
    shuffleBut->setFixedSize(QSize(25, 25));
    loopBut->setFixedSize(QSize(25, 25));
    showStartupMessage();
    ui->volumeSlider->setValue(appSettings.value("volumeLevel").toInt());
    initializeButtonTheme();
    int my_argc = qApp->arguments().count();
    if(my_argc >= 2){
        runtimeFile = qApp->arguments().at(1);
        fileName = runtimeFile;
        QFileInfo file(fileName);
        if(file.completeSuffix().toLower() == "flac" || file.completeSuffix().toLower() == "mp3" || file.completeSuffix().toLower() == "ogg" || file.completeSuffix().toLower() == "wav" || file.completeSuffix().toLower() == "mod"){
            player->play(fileName);
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            player->changeVolume(floatVolume);
            ui->playButton->setIcon(QIcon(":/images/resources/buttonPause" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
        }
        else{
            messageBox.critical(0,"Invalid File","Please load a supported file!");
            messageBox.setFixedSize(500,200);
            fileName = "";
        }

    }
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}

//TODO: remove some debug functions that arent needed anymore
int getRand(int min, int max){
    std::uniform_int_distribution<> uid(min, max);
    return uid(*QRandomGenerator::global());
}

//got this piece of code from qt's media player example
static QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

void MusicPlayer::initializeButtonTheme(){
    QString curTheme = currentTheme.replace(0, 1, currentTheme[0].toUpper());
    ui->playButton->setIcon(QIcon(":/images/resources/buttonPlay" + curTheme + ".png"));
    ui->stopButton->setIcon(QIcon(":/images/resources/buttonStop" + curTheme + ".png"));
    ui->fowardButton->setIcon(QIcon(":/images/resources/buttonFoward" + curTheme + ".png"));
    ui->rewindButton->setIcon(QIcon(":/images/resources/buttonRewind" + curTheme + ".png"));
    if(ui->volumeSlider->value() >= 1){
        ui->volumeButton->setIcon(QIcon(":/images/resources/volumeLevel" + curTheme + ".png"));
    }
    else{
        ui->volumeButton->setIcon(QIcon(":/images/resources/volumeDown" + curTheme + ".png"));
    }
}

void MusicPlayer::showStartupMessage(){
    ui->statusbar->addPermanentWidget(label, 1);
    ui->statusbar->addPermanentWidget(shuffleBut);
    ui->statusbar->addPermanentWidget(loopBut);
    label->setText(startupMessages.at(getRand(0,5)));
}

void MusicPlayer::on_actionOpen_Song_triggered()
{
    if(player->playing == true){
        player->stop();
    }
    QString soundPath = QFileDialog::getOpenFileName(this,tr("Open Audio File"), "", tr("Audio Files (*.mp3 *.wav *.flac *.ogg *.mod)"));
    if(isPlaylist == true){
        isPlaylist = false;
        playlistsView->clearList();
        for(int idx = 0; idx < song_list.size(); ++idx)
        {
            song_list.remove(idx);
            --idx;
        }
    }
    if (soundPath == ""){
        return;
    }
    else{
        if(QFileInfo(soundPath).completeSuffix() == "mod"){
            isMod = true;
        }
        fileName = QFileInfo(soundPath).filePath();
        player->playing = false;
    }
}

void MusicPlayer::checkPlaybackState(){
    if(player->playing == true){
        ui->playButton->setIcon(QIcon(":/images/resources/buttonPause" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
    }
    else{
        ui->playButton->setIcon(QIcon(":/images/resources/buttonPlay" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
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
           ui->playButton->setIcon(QIcon(":/images/resources/buttonPause" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
       }
       else{
           player->pause();
           ui->playButton->setIcon(QIcon(":/images/resources/buttonPlay" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
       }

    }
}

void MusicPlayer::updatePositionSlider(){
    ui->progressSlider->setValue(player->currentPosition());
    label->setText(formatTime(player->currentPosSeconds()) + " / " + formatTime(player->audioLengthSeconds()));
}

void MusicPlayer::updateDuration(){
    ui->progressSlider->setMaximum(player->audioLength());
}

//WIP!
//For anybody who knows C++ and knows how to use TagLib and Qt, please help me fix this big issue I have, I would be really happy.

QImage updateImage(QString path){
    QFileInfo songExt(path);
    if(songExt.exists() == true){
        if(songExt.completeSuffix() == "flac"){
            QImage coverArt;
            TagLib::FLAC::File *flacFile = new TagLib::FLAC::File(path.toLatin1().toStdString().c_str());
            TagLib::List<TagLib::FLAC::Picture*> picList = flacFile->pictureList();
            if(picList.isEmpty()){
                return coverArt;
            }
            else{
                TagLib::FLAC::Picture *mainPic = picList[0];
                coverArt.loadFromData(QByteArray(mainPic->data().data(), mainPic->data().size()));
                return coverArt;
            }
        }
        else if(songExt.completeSuffix() == "mp3"){
            QImage coverArt;
            TagLib::MPEG::File *mpFile = new TagLib::MPEG::File(path.toLatin1().toStdString().c_str());
            TagLib::ID3v2::Tag *m_tag = mpFile->ID3v2Tag();
            auto frameList = m_tag->frameList("APIC");
            if (m_tag && !frameList.isEmpty()){
                TagLib::ID3v2::AttachedPictureFrame *coverImg = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());
                if(coverArt.loadFromData((const uchar *)coverImg->picture().data(), coverImg->picture().size())){
                    qInfo() << "Image did load!";
                    return coverArt;
                }
            }
            else{
                return coverArt;
            }
            delete mpFile;
        }
    }
    return QImage();
}

void MusicPlayer::updateInfo()
{
    QStringList info;
    if (!fileName.isEmpty()){
        TagLib::FileRef *fileRef = new TagLib::FileRef(fileName.toLatin1().toStdString().c_str());
        TagLib::Tag *tag = fileRef->tag();
        TagLib::String taglibArtist(tag->artist());
        TagLib::String taglibTitle(tag->title());
        QString author = QString::fromWCharArray(taglibArtist.toCWString());
        if (!author.isEmpty()){
            info.append(author);
        }
        else{
            info.append("Unknown Artist");
        }
        QString title = QString::fromWCharArray(taglibTitle.toCWString());
        if (!title.isEmpty()){
            info.append(title);
        }
        else{
            info.append("Unknown Song");
        }
        delete fileRef;

    }
    ui->songName->setText(info.join(tr(" - ")));
}


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
        playlistsView->clearList();
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
                    playlistsView->addSong(fileThingie.completeBaseName());
                    qInfo() << value.toString();
                    songIndex = pl_entries.count() - pl_entries.count();
                    maxIndex = pl_entries.count();
                    qInfo() << songIndex;
                    qInfo() << maxIndex;
                    qInfo() << song_list;
                }

            }
            playlistsView->setSong(songIndex);
            fileName = song_list.at(songIndex);          
            isPlaylist = true;
            player->playing = false;
        }
    }
}


void MusicPlayer::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
   songIndex = playlistsView->currentSelection();
   if(player->loaded == true){
       player->stop();
   }
   fileName = song_list.at(songIndex);

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
        if(songIndex <= playlistsView->totalIndex() - 2){
            if(isShuffle == true){
                previousIndex = songIndex;
                songIndex = getRand(0, maxIndex - 1);
            }
            else{
                songIndex++;
            }
            qInfo() << songIndex;
            qInfo() <<  playlistsView->totalIndex() - 2;
            playlistsView->setSong(songIndex);
            if(player->loaded == true){
                player->stop();
            }
            fileName = song_list.at(songIndex);

            player->play(fileName);
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            player->changeVolume(floatVolume);
        }
        else{
            if(isShuffle == true){
                songIndex = getRand(0, maxIndex);
                playlistsView->setSong(songIndex);
                if(player->loaded == true){
                    player->stop();
                }
                fileName = song_list.at(songIndex);

                player->play(fileName);
                float floatVolume = (float)ui->volumeSlider->value() / 100;
                player->changeVolume(floatVolume);
            }
            if(isLoop == true){
                songIndex = 0;
                playlistsView->setSong(songIndex);
                if(player->loaded == true){
                    player->stop();
                }
                fileName = song_list.at(songIndex);

                player->play(fileName);
                float floatVolume = (float)ui->volumeSlider->value() / 100;
                player->changeVolume(floatVolume);
            }
            return;
        }
    }
}


void MusicPlayer::on_rewindButton_clicked()
{
    if(isPlaylist == true){
        if(songIndex > 0){
            if(isShuffle == true){
                if(songIndex == previousIndex){
                    songIndex = getRand(0, maxIndex);
                }
                else{
                    songIndex = previousIndex;
                }
            }
            else{
                songIndex--;
            }
            playlistsView->setSong(songIndex);
            if(player->loaded == true){
                player->stop();
            }
            fileName = song_list.at(songIndex);

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
    if(isPlaylist == true){
        if(songIndex <= playlistsView->totalIndex() - 2){
            if(isShuffle == true){
                previousIndex = songIndex;
                songIndex = getRand(0, maxIndex);
            }
            else{
                songIndex++;
            }
            qInfo() << songIndex;
            qInfo() <<  playlistsView->totalIndex() - 2;
            playlistsView->setSong(songIndex);
            if(player->loaded == true){
                player->stop();
            }
            fileName = song_list.at(songIndex);

            player->play(fileName);
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            player->changeVolume(floatVolume);
        }
        else{
            if(isLoop == true){
                if(isShuffle == true){
                    songIndex = getRand(0, maxIndex);
                }
                else{
                    songIndex = 0;
                }
                if(player->loaded == true){
                    player->stop();
                }
                fileName = song_list.at(songIndex);
                playlistsView->setSong(songIndex);

                player->play(fileName);
                float floatVolume = (float)ui->volumeSlider->value() / 100;
                player->changeVolume(floatVolume);
            }
            else{
                return;
            }
        }
    }
    else{
        if(isLoop == true){
            if(player->loaded == true){
                player->stop();
            }
            player->play(fileName);
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            player->changeVolume(floatVolume);
        }
        else{
            player->stop();
        }
    }
}


void MusicPlayer::on_volumeSlider_sliderReleased()
{
    QSettings appSettings("AM_Erizur", "FrutaGroovePlayer");
    appSettings.setValue("volumeLevel", ui->volumeSlider->value());
    qInfo() << appSettings.value("volumeLevel");
}

void MusicPlayer::on_actionAbout_triggered()
{
    abInf = new aboutInfo(); // Be sure to destroy your window somewhere
    abInf->show();
}

void MusicPlayer::handleError(int code)
{
    switch(code){
        case 1:
            messageBox.critical(0,"Couldn't load output","There was an error while initializing the audio device! \nMake sure you have an audio output properly connected.");
            messageBox.setFixedSize(500,200);
            QCoreApplication::exit();
            break;
        case 2:
            messageBox.critical(0,"Plugin Load Failed","Make sure the program is installed correctly and try again.");
            messageBox.setFixedSize(500,200);
            QCoreApplication::exit();
            break;
        case 3:
            messageBox.warning(0,"Invalid File","There was an error loading the file, please make sure it is supported.");
            messageBox.setFixedSize(500,200);
            break;
        case 4:
            messageBox.critical(0,"Error resuming playback","There was an error while trying to resume playback.");
            messageBox.setFixedSize(500,200);
            player->stop();
            break;
        default:
            messageBox.critical(0,"Unknown Error","This is an unknown error, please report it on the GitHub.");
            messageBox.setFixedSize(500,200);
    }
}


void MusicPlayer::on_actionLoop_triggered(bool checked)
{
    if(checked){
        isLoop = true;
    }
    else{
        isLoop = false;
    }
    qInfo() << "THING IS: " << isLoop << " ALSO IS " << checked;
}

void MusicPlayer::on_actionShuffle_triggered(bool checked)
{
    if(checked){
        isShuffle = true;
    }
    else{
        isShuffle = false;
    }
    qInfo() << "THING IS: " << isShuffle << " ALSO IS " << checked;
}


void MusicPlayer::on_volumeSlider_sliderMoved(int position)
{
    if(position >= 1){
        ui->volumeButton->setIcon(QIcon(":/images/resources/volumeLevel" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
    }
    else{
        ui->volumeButton->setIcon(QIcon(":/images/resources/volumeDown" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
    }
}


void MusicPlayer::on_volumeButton_clicked()
{
    QSettings appSettings("AM_Erizur", "FrutaGroovePlayer");
    if(ui->volumeSlider->value() >= 1){
        appSettings.setValue("previousVolume", ui->volumeSlider->value());
        ui->volumeSlider->setValue(0);
        player->changeVolume(0);
        ui->volumeButton->setIcon(QIcon(":/images/resources/volumeDown" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
    }
    else{
        if(appSettings.value("previousVolume").isValid()){
            ui->volumeSlider->setValue(appSettings.value("previousVolume").toInt());
            QSettings appSettings("AM_Erizur", "FrutaGroovePlayer");
            appSettings.setValue("volumeLevel", ui->volumeSlider->value());
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            player->changeVolume(floatVolume);
            ui->volumeButton->setIcon(QIcon(":/images/resources/volumeLevel" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
        }
        else{
            ui->volumeSlider->setValue(100);
            QSettings appSettings("AM_Erizur", "FrutaGroovePlayer");
            appSettings.setValue("volumeLevel", ui->volumeSlider->value());
            float floatVolume = (float)ui->volumeSlider->value() / 100;
            player->changeVolume(floatVolume);
            ui->volumeButton->setIcon(QIcon(":/images/resources/volumeLevel" + currentTheme.replace(0, 1, currentTheme[0].toUpper()) + ".png"));
        }
    }
}

void MusicPlayer::on_actionPlaylists_triggered()
{
    plViewHandler->setTitle(playlistsView->windowTitle());
    plViewHandler->setIcon(playlistsView->windowIcon());
    plViewHandler->setContent(playlistsView);
    plViewHandler->setSize(playlistsView->size());
    plViewHandler->show();
}


void MusicPlayer::on_actionVisualizer_triggered()
{
    metaViewHandler->setTitle(metadataView->windowTitle());
    metaViewHandler->setIcon(metadataView->windowIcon());
    metaViewHandler->setContent(metadataView);
    metaViewHandler->setSize(metadataView->size());
    metaViewHandler->show();
}

