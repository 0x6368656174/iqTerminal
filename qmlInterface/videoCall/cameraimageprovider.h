#ifndef ImageProvider_H
#define ImageProvider_H

#include <QQuickImageProvider>

class CameraImageProvider : public QQuickImageProvider
{
public:
    explicit CameraImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) ,currentCameraIn(QImage()),currentCameraOut(QImage()) {}

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize)
    {
        QImage result;
        if(id.mid(0,16)=="CurrentCameraOut")
        {
            if (requestedSize.isValid()) {
                result = currentCameraOut.scaled(requestedSize, Qt::KeepAspectRatio);
            } else {
                result = currentCameraOut;
            }
            *size = result.size();
        } else
            if(id.mid(0,15)=="CurrentCameraIn")
            {
                if (requestedSize.isValid()) {
                    result = currentCameraIn.scaled(requestedSize, Qt::KeepAspectRatio);
                } else {
                    result = currentCameraIn;
                }
                *size = result.size();
            } else
                {
                    result = QImage();
                    *size = result.size();
                }
            return result;
    }

public slots:
    void setCurrentCameraInSlot(const QImage & image)
    {
        if(!image.isNull()) currentCameraIn = image.copy();
    }
    void setCurrentCameraOutSlot(const QImage & image)
    {
        if(!image.isNull()) currentCameraOut = image.copy();
    }

private:
    QImage currentCameraIn;
    QImage currentCameraOut;
};

#endif // ImageProvider_H
