#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QTextCodec>
#include "bass24/c/bass.h"

void __stdcall syncFunc(HSYNC handle, DWORD channel, DWORD data, void *user);

class AudioThread : public QThread
{
    Q_OBJECT
public:
    explicit AudioThread(QObject *parent = 0);
    unsigned long chan;
    bool playing;
    bool loaded;
    void run();
private:
    QTimer *t;
signals:
    void endOfPlayback();
    void curPos(double Position, double Total);
    void positionChanged();
    void lengthChanged();
    void stateChanged();
    void metadataChanged();
    void errorHappened(int code);
public slots:
    void play(QString filepath);
    void pause();
    void resume();
    void stop();
    void signalUpdate();
    void changePosition(int position);
    void changeVolume(float volume);
    int currentPosition();
    int audioLength();
    int currentPosSeconds();
    int audioLengthSeconds();
};
#endif // AUDIOTHREAD_H
