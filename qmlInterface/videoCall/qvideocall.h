#ifndef QVIDEOCALL_H
#define QVIDEOCALL_H

#include <QObject>
#include <QQmlContext>

#include <QCamera>
#include <QVideoDeviceSelectorControl>
#include <QDesktopServices>
#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>

#include "cameraimageprovider.h"

class QVideoCall : public QObject
{
    Q_OBJECT
public:
    explicit QVideoCall(QObject *parent = 0);
    CameraImageProvider *cameraImageProvider; // Провайдер для отображения изменяющихся во времени картинок

signals:
    // внутренний сигнал
    void repaintCurrentCameraInSelfSignalQml(); // Обновление картинки в Qml

    // внешний сигнал
    void updateCurCameraOutSignal(QImage); // Передаем оппоненту картинку

public slots:
    // внешние слоты
    void updateCurCameraInSlot(const QImage & curImage); // Получаем картинку от оппонента
    void startCurCameraOutFromBeginSlot();  // Обновить получаемую картинку с полной версии

    // внутренние слоты

    //Слот для показа экрана
    void updateCurCameraOutSlot();

    //Слоты для видеокамеры
    void stopCameraOutSlot();
    void updateCurCameraOutSlot(const QImage & curImage, int);
    int choosedCamera();
    void setChoosedCamera(int);
    void setSelectedCameraDevice(int);
    void setCameraObject(QObject *cam);

private:
    int curChoosedCamera;
    bool newStartVideoSignal;
    QCamera *m_camera;
    QVideoDeviceSelectorControl *m_deviceSelector;

    QImage currentCamera;
    QImage currentCameraIn;
    QImage currentCameraInOld;
    QImage currentCameraInDiff;
    QImage currentCameraOut;
    QImage currentCameraOutOld;
    QImage currentCameraOutDiff;
    QImage defaultInfoImg;

};

#endif // QVIDEOCALL_H
