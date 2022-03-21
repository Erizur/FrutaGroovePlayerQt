#include "audiothread.h"

bool endOfMusic;

void __stdcall syncFunc(HSYNC handle, DWORD channel, DWORD data, void *user)
{
    BASS_ChannelRemoveSync(channel, handle);
    BASS_ChannelStop(channel);
    qDebug() << "End of playback!";
    endOfMusic = true;
}

AudioThread::AudioThread(QObject *parent) :
    QThread(parent)
{
    if (!BASS_Init(-1, 44100, 0, NULL, NULL))
        qDebug() << "Cannot initialize device";
        emit errorHappened(1);
    if (!BASS_PluginLoad("bassflac.dll", 0) && !BASS_PluginLoad("bassopus.dll", 0))
        qInfo() << "ERROR CODE: " << BASS_ErrorGetCode();
        emit errorHappened(2);
    t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(signalUpdate()));
    endOfMusic = true;
}

void AudioThread::play(QString filename)
{
    BASS_ChannelStop(chan);
    if (!(chan = BASS_StreamCreateFile(false, filename.toLatin1(), 0, 0, BASS_SAMPLE_LOOP)) && !(chan = BASS_MusicLoad(false, filename.toLatin1(), 0, 0, BASS_MUSIC_RAMP /*| BASS_SAMPLE_LOOP */ | BASS_MUSIC_PRESCAN, 1))){
        qDebug() << "Can't play file due to error code: " << BASS_ErrorGetCode();
        emit errorHappened(3);
    }
    else
    {
        endOfMusic = false;
        emit metadataChanged();
        BASS_ChannelPlay(chan, true);
        loaded = true;
        emit lengthChanged();
        t->start(100);
        BASS_ChannelSetSync(chan, BASS_SYNC_END, 0, &syncFunc, 0);
        playing = true;
        emit stateChanged();
    }
}

void AudioThread::pause()
{
    BASS_ChannelPause(chan);
    t->stop();
    playing = false;
    emit stateChanged();
}

void AudioThread::resume()
{
    if (!BASS_ChannelPlay(chan, false)){
        qDebug() << "Error resuming";
        emit errorHappened(4);
    }
    else
    {
        t->start(98);
        playing = true;
        emit stateChanged();
    }
}

void AudioThread::stop()
{
    BASS_ChannelStop(chan);
    t->stop();
    playing = false;
    loaded = false;
    emit stateChanged();
}

void AudioThread::signalUpdate()
{
    if (endOfMusic == false)
    {
        emit curPos(BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE)),
                    BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetLength(chan, BASS_POS_BYTE)));
        emit positionChanged();
    }
    else
    {
        emit endOfPlayback();
        playing = false;
        loaded = false;
    }
}

void AudioThread::changePosition(int position)
{
    BASS_ChannelSetPosition(chan, position, BASS_POS_BYTE);
}

void AudioThread::changeVolume(float volume)
{
    BASS_ChannelSetAttribute(chan, BASS_ATTRIB_VOL, volume);
}

int AudioThread::currentPosition(){
    int pos = BASS_ChannelGetPosition(chan, BASS_POS_BYTE);
    return pos;
}

int AudioThread::audioLength(){
    int length = BASS_ChannelGetLength(chan, BASS_POS_BYTE);
    return length;
}

int AudioThread::currentPosSeconds(){
    int pos = BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetPosition(chan, BASS_POS_BYTE));
    return pos * 1000;
}

int AudioThread::audioLengthSeconds(){
    int length = BASS_ChannelBytes2Seconds(chan, BASS_ChannelGetLength(chan, BASS_POS_BYTE));
    return length * 1000;
}

void AudioThread::run()
{
    while (1);
}
