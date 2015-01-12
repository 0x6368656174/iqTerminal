#include "myringtone.h"
#include <QDebug>

MyRingtone::MyRingtone(QObject *parent) :
    QObject(parent)
{
    audio=NULL;
}

void MyRingtone::play(QString FileName)
{
    sourceFile.setFileName(FileName);
    sourceFile.open(QIODevice::ReadOnly);

    char buffer[44];
    WaveHeader *hdr=(WaveHeader*)buffer;

    sourceFile.read(buffer,44);

    QAudioFormat format;
    format.setSampleRate(hdr->sampleRate);
    format.setChannelCount(hdr->numChannels);
    format.setSampleSize(hdr->bitsPerSample);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

//    qDebug() << "ringtone" << hdr->sampleRate << hdr->numChannels << hdr->bitsPerSample;
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        sourceFile.close();
        return;
    }

    audio = new QAudioOutput(format, this);
    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioStateChanged(QAudio::State)));
    audio->start(&sourceFile);
}

void MyRingtone::audioStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            audio->stop();
            break;

        case QAudio::StoppedState:
            // Stopped for other reasons
            if (audio->error() != QAudio::NoError) {
                delete audio;
                sourceFile.close();
                emit stopped(this);
            }
            break;

        default:
            break;
    }
}
