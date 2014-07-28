#include "audiorecorder.h"

AudioRecorder::AudioRecorder(QObject *parent) :
    QObject(parent),
    _recored(new QAudioRecorder(this))
{
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/mp3");
    audioSettings.setQuality(QMultimedia::NormalQuality);
    _recored->setAudioSettings(audioSettings);
}


void AudioRecorder::record()
{
    _recored->record();
}

void AudioRecorder::stop()
{
    _recored->stop();

    emit outputLocationChanged();
}
