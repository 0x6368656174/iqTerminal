#include "qvideocall.h"


QVideoCall::QVideoCall(QObject *parent) : QObject(parent)
  ,curChoosedCamera(0)
  ,newStartVideoSignal(true)
{
    cameraImageProvider = new CameraImageProvider();

    // картинки
    QImage defaultImage(":/defaultimage.png"); // Картинка по умолчанию
    currentCameraIn = defaultImage.copy();
    cameraImageProvider->setCurrentCameraInSlot(currentCameraIn);
    currentCameraOut = defaultImage.copy();
    cameraImageProvider->setCurrentCameraOutSlot(currentCameraOut);
    defaultInfoImg = defaultImage.copy();
}

int nDelta=10; // нечуствительность к изменениям в картинке 5-15
int redDelta=0;
int greenDelta=0;
int blueDelta=0;
int alphaDelta=0;
bool CompareBit(QRgb c1, QRgb c2, int nDelta)
{
    if(qRed(c1) > qRed(c2)) redDelta = qRed(c1)-qRed(c2); else redDelta = qRed(c2)-qRed(c1);
    if(qGreen(c1) > qGreen(c2)) greenDelta = qGreen(c1)-qGreen(c2); else greenDelta = qGreen(c2)-qGreen(c1);
    if(qBlue(c1) > qBlue(c2)) blueDelta = qBlue(c1)-qBlue(c2); else blueDelta = qBlue(c2)-qBlue(c1);
    if ((redDelta < nDelta)&&(greenDelta < nDelta)&&(blueDelta < nDelta)) return true;
    return false;
}
bool CompareBitStronge(QRgb c1, QRgb c2)
{
    if ((qRed(c1) == qRed(c2))&&(qGreen(c1) == qGreen(c2))&&(qBlue(c1) == qBlue(c2))) return true;
    //if ((qRed(c1) == qRed(c2))&&(qGreen(c1) == qGreen(c2))&&(qBlue(c1) == qBlue(c2))&&(qAlpha(c1) == qAlpha(c2))) return true;
    return false;
}
QImage ImageDiffDelta(QImage oldImage, QImage currentImage, int nDeltaSet)
{
    nDelta = nDeltaSet;
    if(nDelta<5) nDelta=5;
    if(nDelta>15) nDelta=15;
    QImage diffImage = currentImage.copy();

    if(oldImage.size() == currentImage.size())
    {
        if((diffImage.width()==oldImage.width())&&(diffImage.height()==oldImage.height()))
        {
            int pixels = diffImage.width() * diffImage.height();
            if (pixels*(int)sizeof(QRgb) <= diffImage.byteCount())
            {
                QRgb *dataOld = (QRgb *)oldImage.bits();
                QRgb *dataDiff = (QRgb *)diffImage.bits();
                for (int i = 0; i < pixels; i++)
                {
                    if (CompareBit(dataOld[i], dataDiff[i], nDelta)) dataDiff[i] = qRgba(0, 0, 0, 0);
                }
            }
        }
    }
    return diffImage;
}
QImage ImageRestore(QImage currentImage, QImage diffImage)
{
    QImage restoredImage;
    if(currentImage.size() == diffImage.size())
    {
        restoredImage = currentImage.copy();
        if((restoredImage.width()==currentImage.width())&&(restoredImage.height()==currentImage.height()))
        {
            int pixels = restoredImage.width() * restoredImage.height();
            if (pixels*(int)sizeof(QRgb) <= restoredImage.byteCount())
            {
                QRgb *dataCur = (QRgb *)restoredImage.bits();
                QRgb *dataDiff = (QRgb *)diffImage.bits();
                for (int i = 0; i < pixels; i++)
                {
                    if(!CompareBitStronge(dataDiff[i],qRgba(0, 0, 0, 0))) dataCur[i] = dataDiff[i];
                }
            }
        }
    } else {
        restoredImage = diffImage.copy();
    }
    return restoredImage;
}

void QVideoCall::startCurCameraOutFromBeginSlot()
{
    newStartVideoSignal=true;
}

void QVideoCall::updateCurCameraOutSlot()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    QDesktopWidget* desktop = QApplication::desktop();
    QPixmap screenPixmap = screen->grabWindow( desktop->screen()->winId() );
    QImage screenImage = screenPixmap.toImage().scaled(QSize(640,480),Qt::KeepAspectRatio);
    if(!screenImage.isNull())
    {
        currentCameraOutOld = currentCameraOut;
        currentCameraOut = screenImage.copy();
        cameraImageProvider->setCurrentCameraOutSlot(screenImage);
        emit updateCurCameraOutSignal(currentCameraOut);

//        currentCameraOutDiff = ImageDiffDelta(currentCameraOutOld,currentCameraOut,10);

//        if(newStartVideoSignal)
//            emit cameraImageProvider->setCurrentCameraOutSlot(currentCameraOut);
//        else
//            emit cameraImageProvider->setCurrentCameraOutSlot(currentCameraOutDiff);

//        if(newStartVideoSignal)
//            emit updateCurCameraOutSignal(currentCameraOut);
//        else
//            emit updateCurCameraOutSignal(currentCameraOutDiff);
        newStartVideoSignal=false;
    }
}

void QVideoCall::stopCameraOutSlot()
{
    if(!defaultInfoImg.isNull())
    {
        currentCameraOut = defaultInfoImg.copy();
        cameraImageProvider->setCurrentCameraOutSlot(defaultInfoImg);
        emit updateCurCameraOutSignal(currentCameraOut);
    }
    //qDebug() << "Stop Video Out";
}

void QVideoCall::updateCurCameraOutSlot(const QImage &curImage, int newDelta)
{
//    qDebug() << "update camera";
    if(!curImage.isNull())
    {
        currentCameraOutOld = currentCameraOut;
        currentCameraOut = curImage.copy();
        cameraImageProvider->setCurrentCameraOutSlot(curImage);
        emit updateCurCameraOutSignal(currentCameraOut);

        curImage.~QImage();
//        currentCameraOutDiff = ImageDiffDelta(currentCameraOutOld, currentCameraOut, newDelta);

//        if(newStartVideoSignal)
//            emit cameraImageProvider->setCurrentCameraOutSlot(currentCameraOut);
//        else
//            emit cameraImageProvider->setCurrentCameraOutSlot(currentCameraOutDiff);

//        if(newStartVideoSignal)
//            emit updateCurCameraOutSignal(currentCameraOut);
//        else
//            emit updateCurCameraOutSignal(currentCameraOutDiff);
        newStartVideoSignal=false;
    }
}

void QVideoCall::updateCurCameraInSlot(const QImage &curImage)
{
    if(!curImage.isNull())
    {
        currentCameraInOld = currentCameraIn;
//        currentCameraInDiff = curImage.copy();
//        QImage image = ImageRestore(currentCameraInOld,currentCameraInDiff);
        QImage image = curImage.copy();
        currentCameraIn = image;
        cameraImageProvider->setCurrentCameraInSlot(image);
        emit repaintCurrentCameraInSelfSignalQml();
    }
}

int QVideoCall::choosedCamera()
{
    return curChoosedCamera;
}

void QVideoCall::setChoosedCamera(int newChoosedCamera)
{
    curChoosedCamera = newChoosedCamera;
}

void QVideoCall::setSelectedCameraDevice(int action)
{
    int cameraId = 0;
    int current = 0;
    current = m_deviceSelector->selectedDevice();
    switch (action) {
    case 0:
        cameraId=m_deviceSelector->defaultDevice();
        if((choosedCamera()>0)&&(m_deviceSelector->deviceCount()>=choosedCamera()))
        {
            cameraId=choosedCamera();
        }
        break;
    case 1:
        if(m_deviceSelector->deviceCount()>(current+1))
        {
            cameraId=current+1;
        }
        break;
    default:
        break;
    }
    m_camera->unload();
    m_deviceSelector->setSelectedDevice(cameraId);
    setChoosedCamera(cameraId);
}

void QVideoCall::setCameraObject(QObject *camera)
{
    qDebug() << "setCameraObject";
    m_camera = qvariant_cast<QCamera*>(camera->property("mediaObject"));
    QMediaService *service = m_camera->service();
    m_deviceSelector = qobject_cast<QVideoDeviceSelectorControl*>(service->requestControl(QVideoDeviceSelectorControl_iid));
}
