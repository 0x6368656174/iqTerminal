#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QUrl>
#include <QAudioRecorder>

class AudioRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl outputLocation READ outputLocation NOTIFY outputLocationChanged)
public:
    explicit AudioRecorder(QObject *parent = 0);

    Q_INVOKABLE void record();

    Q_INVOKABLE void stop();

public:
    inline QUrl outputLocation() const {return _recored->outputLocation();}

signals:
    void outputLocationChanged();

private:
    QAudioRecorder *_recored;
};

#endif // AUDIORECORDER_H
