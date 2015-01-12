#ifndef MYRINGTONE_H
#define MYRINGTONE_H

#include <QObject>
#include <QFile>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

#pragma pack(1)
typedef struct {
    char   chunkid[4];
    qint32 chunksize;
    char   format[4];
    char   subchunk1id[4];
    qint32 subchunk1size;
    qint16 audioformat;
    qint16 numChannels;
    qint32 sampleRate;
    qint32 byteRate;
    qint16 blockAlign;
    qint16 bitsPerSample;
    char   subchunk2id[4];
    qint32 subchunk2size;
} WaveHeader;
#pragma pack(8)

class MyRingtone : public QObject
{
    Q_OBJECT

    QFile sourceFile;    // class member.
    QAudioOutput* audio; // class member.

public:
    explicit MyRingtone(QObject *parent = 0);

    void play(QString FileName);

signals:
    void stopped(MyRingtone *);

public slots:
    void audioStateChanged(QAudio::State newState);
};

#endif // MYRINGTONE_H
