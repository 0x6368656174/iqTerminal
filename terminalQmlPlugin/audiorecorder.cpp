#include "audiorecorder.h"

AudioRecorder::AudioRecorder(QObject *parent) :
    QObject(parent),
    m_recored(new QAudioRecorder(this))
{
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/mp3");
    audioSettings.setQuality(QMultimedia::NormalQuality);
    m_recored->setAudioSettings(audioSettings);
}


void AudioRecorder::record()
{
    m_recored->record();
}

void AudioRecorder::stop()
{
    m_recored->stop();

    emit outputLocationChanged();
}

QUrl AudioRecorder::outputLocation() const
{
    return m_recored->outputLocation();
}
