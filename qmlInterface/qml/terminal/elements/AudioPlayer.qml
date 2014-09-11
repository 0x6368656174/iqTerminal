import QtQuick 2.0
import QtMultimedia 5.2

Audio {
    property bool isPlay: false
    property bool isPause: false
    property bool isStop: false

    signal finished(var source)

    onPlaying: {
        isPlay = true
        isPause = false
        isStop = false
    }
    onPaused:  {
        isPlay = false
        isPause = true
        isStop = false
    }
    onStopped: {
        isPlay = false
        isPause = false
        isStop = true
    }


    onStatusChanged: {
        if (status === Audio.EndOfMedia)
            finished(source)
    }
}
