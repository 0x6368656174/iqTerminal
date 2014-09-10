import QtQuick 2.0
import QtMultimedia 5.2

Audio {
    property bool isPlay: false
    property bool isPause: false

    signal finished()

    onPlaying: {
        isPlay = true
        isPause = false
    }
    onPaused:  {
        isPlay = false
        isPause = true
    }
    onStopped: {
        isPlay = false
        isPause = false

        if (duration === position)
            finished()
    }
}
