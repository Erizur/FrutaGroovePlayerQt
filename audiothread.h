#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

#include <QThread>
#include <QDebug>
#include <QTimer>
#include "bass24/c/bass.h"

void __stdcall syncFunc(HSYNC handle, DWORD channel, DWORD data, void *user);

class AudioThread : public QThread
{
    Q_OBJECT
public:
    explicit AudioThread(QObject *parent = 0);
    bool playing;
    bool loaded;
    void run();
private:
    unsigned long chan;
    QTimer *t;
signals:
    void endOfPlayback();
    void curPos(double Position, double Total);
    void positionChanged();
    void lengthChanged();
    void stateChanged();
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
