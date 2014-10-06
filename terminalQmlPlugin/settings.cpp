#include "settings.h"
#include "core.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>

Settings::Settings(QObject *parent) :
    QObject(parent),
    m_source(QUrl()),
    m_parentElement(""),
    m_translation(English),
    m_homeDirectory(Core::homeDir()),
    m_audioDevicesModel(new SimpleDevicesModel(this)),
    m_videoDevicesModel(new SimpleDevicesModel(this)),
    m_wanDevicesModel(new NetworkDevicesModel(this)),
    m_natEnabled(false),
    m_upnpEnabled(false),
    m_proxySettings(new ProxySettings(this)),
    m_zoomFactor(1.0),
    m_applicatonTranslator(nullptr)
{
}

QUrl Settings::source() const
{
    return m_source;
}

void Settings::setSource(const QUrl &source)
{
    if(m_source != source) {
        m_source = source;
        emit sourceChanged();

        if (!parentElement().isEmpty())
            reload();
    }
}

QString Settings::parentElement() const
{
    return m_parentElement;
}

void Settings::setParentElement(const QString &parentElement)
{
    if (m_parentElement != parentElement) {
        m_parentElement = parentElement;
        emit parentElementChanged();

        if (source().isValid())
            reload();
    }
}

Settings::Translations Settings::translation() const
{
    return m_translation;
}

void Settings::setTranslation(const Translations &translation)
{
    if (m_translation != translation) {
        m_translation = translation;

        if (m_applicatonTranslator) {
            QCoreApplication::removeTranslator(m_applicatonTranslator);
            m_applicatonTranslator->deleteLater();
        }

        switch (m_translation) {
        case English:
            m_applicatonTranslator = new QTranslator(this);
            m_applicatonTranslator->load(":/i18n/terminal_en.qm");
            QCoreApplication::installTranslator(m_applicatonTranslator);
            break;
        case Russian:
            m_applicatonTranslator = new QTranslator(this);
            m_applicatonTranslator->load(":/i18n/terminal_ru.qm");
            QCoreApplication::installTranslator(m_applicatonTranslator);
            break;
        }
        emit translationChanged();
    }
}

QUrl Settings::homeDirectory() const
{
    return m_homeDirectory;
}

void Settings::setHomeDirectory(const QUrl &homeDirectory)
{
    if (m_homeDirectory != homeDirectory) {
        m_homeDirectory = homeDirectory;
        emit homeDirectoryChanged();
    }
}

SimpleDevicesModel *Settings::audioDevicesModel() const
{
    return m_audioDevicesModel;
}

SimpleDevicesModel *Settings::videoDevicesModel() const
{
    return m_videoDevicesModel;
}

NetworkDevicesModel *Settings::wanDevicesModel() const
{
    return m_wanDevicesModel;
}

bool Settings::natEnabled() const
{
    return m_natEnabled;
}

void Settings::setNatEnabled(bool natEnabled)
{
    if (m_natEnabled != natEnabled) {
        m_natEnabled = natEnabled;
        emit natEnabledChanged();
    }
}

bool Settings::upnpEnabled() const
{
    return m_upnpEnabled;
}

void Settings::setUpnpEnabled(bool upnpEnabled)
{
    if (m_upnpEnabled != upnpEnabled) {
        m_upnpEnabled = upnpEnabled;
        emit upnpEnabledChanged();
    }
}

ProxySettings *Settings::proxySettings() const
{
    return m_proxySettings;
}

qreal Settings::zoomFactor() const
{
    return m_zoomFactor;
}

void Settings::setZoomFactor(const qreal &zoomFactor)
{
    if (m_zoomFactor != zoomFactor) {
        m_zoomFactor = zoomFactor;
        emit zoomFactorChanged();
    }
}

bool Settings::reload()
{
    QDomDocument domDoc;
    QDomElement rootElement = findElement(this, source(), parentElement(), domDoc);
    if (!rootElement.isNull()) {
        QDomElement translationElement = rootElement.firstChildElement("translation");
        if (!translationElement.isNull()) {
            QString translationText = translationElement.text();
            if (translationText.compare("en", Qt::CaseInsensitive) == 0)
                setTranslation(English);
            else if (translationText.compare("zh", Qt::CaseInsensitive) == 0)
                setTranslation(Chinese);
            else if (translationText.compare("es", Qt::CaseInsensitive) == 0)
                setTranslation(Spanish);
            else if (translationText.compare("jp", Qt::CaseInsensitive) == 0)
                setTranslation(Japanese);
            else if (translationText.compare("pt", Qt::CaseInsensitive) == 0)
                setTranslation(Portuguese);
            else if (translationText.compare("de", Qt::CaseInsensitive) == 0)
                setTranslation(German);
            else if (translationText.compare("ar", Qt::CaseInsensitive) == 0)
                setTranslation(Arabic);
            else if (translationText.compare("fr", Qt::CaseInsensitive) == 0)
                setTranslation(French);
            else if (translationText.compare("ru", Qt::CaseInsensitive) == 0)
                setTranslation(Russian);
            else if (translationText.compare("ko", Qt::CaseInsensitive) == 0)
                setTranslation(Korean);
            else {
                qWarning() << tr("Unknown translation \"%0\". Set to English").arg(translationText);
                setTranslation(English);
            }
        } else {
            setTranslation(English);
        }

        QDomElement homeDirectoryElement = rootElement.firstChildElement("home_directory");
        if (!homeDirectoryElement.isNull()) {
            setHomeDirectory(QUrl(homeDirectoryElement.text()));
        } else {
            setHomeDirectory(Core::homeDir());
        }

        QDomElement natEnabledElement = rootElement.firstChildElement("nat_enabled");
        if (!natEnabledElement.isNull()) {
            setNatEnabled(natEnabledElement.text().compare("true", Qt::CaseInsensitive) == 0?true:false);
        } else {
            setNatEnabled(false);
        }

        QDomElement upnpEnabledElement = rootElement.firstChildElement("upnp_enabled");
        if (!upnpEnabledElement.isNull()) {
            setUpnpEnabled(upnpEnabledElement.text().compare("true", Qt::CaseInsensitive) == 0?true:false);
        } else {
            setUpnpEnabled(false);
        }

        QDomElement zoomFactorElement = rootElement.firstChildElement("zoom_factor");
        if (!zoomFactorElement.isNull()) {
            setZoomFactor(zoomFactorElement.text().toFloat());
        } else {
            setZoomFactor(1.0);
        }

        QDomElement audioDevicesElement = rootElement.firstChildElement("audio_devices");
        if (!audioDevicesElement.isNull()) {
            m_audioDevicesModel->loadFromDomElement(audioDevicesElement);
        } else {
            m_audioDevicesModel->removeRows(0, m_audioDevicesModel->rowCount());
        }

        QDomElement videoDevicesElement = rootElement.firstChildElement("video_devices");
        if (!videoDevicesElement.isNull()) {
            m_videoDevicesModel->loadFromDomElement(videoDevicesElement);
        } else {
            m_videoDevicesModel->removeRows(0, m_videoDevicesModel->rowCount());
        }

        QDomElement wanDevicesElement = rootElement.firstChildElement("wan_devices");
        if (!wanDevicesElement.isNull()) {
            m_wanDevicesModel->loadFromDomElement(wanDevicesElement);
        } else {
            m_wanDevicesModel->removeRows(0, m_wanDevicesModel->rowCount());
        }

        QDomElement proxySettingsElement = rootElement.firstChildElement("proxy_settings");
        if (!proxySettingsElement.isNull()) {
            m_proxySettings->loadFromDomElement(proxySettingsElement);
        } else {
            m_proxySettings->reset();
        }
        return true;
    }

    setTranslation(English);
    setHomeDirectory(Core::homeDir());
    setNatEnabled(false);
    setUpnpEnabled(false);
    setZoomFactor(1.0);
    m_audioDevicesModel->removeRows(0, m_audioDevicesModel->rowCount());
    m_videoDevicesModel->removeRows(0, m_videoDevicesModel->rowCount());
    m_wanDevicesModel->removeRows(0, m_wanDevicesModel->rowCount());
    m_proxySettings->reset();

    return false;
}

bool Settings::save() const
{
    QDomDocument domDoc;
    QDomElement rootElement = createElement(this, source(), parentElement(), domDoc);
    if (!rootElement.isNull()) {
        //Пересоздадим основной элемент
        QDomElement oldRootElement = rootElement;
        rootElement = domDoc.createElement(rootElement.tagName());
        oldRootElement.parentNode().appendChild(rootElement);
        oldRootElement.parentNode().removeChild(oldRootElement);

        QDomElement translationElement = domDoc.createElement("translation");
        rootElement.appendChild(translationElement);
        QDomText translationText;
        switch (translation()) {
        case English:
            translationText = domDoc.createTextNode("en");
            break;
        case Chinese:
            translationText = domDoc.createTextNode("zh");
            break;
        case Spanish:
            translationText = domDoc.createTextNode("es");
            break;
        case Japanese:
            translationText = domDoc.createTextNode("jp");
            break;
        case Portuguese:
            translationText = domDoc.createTextNode("pt");
            break;
        case German:
            translationText = domDoc.createTextNode("de");
            break;
        case Arabic:
            translationText = domDoc.createTextNode("ar");
            break;
        case French:
            translationText = domDoc.createTextNode("fr");
            break;
        case Russian:
            translationText = domDoc.createTextNode("ru");
            break;
        case Korean:
            translationText = domDoc.createTextNode("ko");
            break;
        }
        translationElement.appendChild(translationText);

        QDomElement homeDirectoryElement = domDoc.createElement("home_directory");
        rootElement.appendChild(homeDirectoryElement);
        QDomText homeDirectoryText = domDoc.createTextNode(homeDirectory().toString());
        homeDirectoryElement.appendChild(homeDirectoryText);

        QDomElement natEnabledElement = domDoc.createElement("nat_enabled");
        rootElement.appendChild(natEnabledElement);
        QDomText natEnabledText = domDoc.createTextNode(natEnabled()?"true":"false");
        natEnabledElement.appendChild(natEnabledText);

        QDomElement upnpEnabledElement = domDoc.createElement("upnp_enabled");
        rootElement.appendChild(upnpEnabledElement);
        QDomText upnpEnabledText = domDoc.createTextNode(upnpEnabled()?"true":"false");
        upnpEnabledElement.appendChild(upnpEnabledText);

        QDomElement zoomFactorElement = domDoc.createElement("zoom_factor");
        rootElement.appendChild(zoomFactorElement);
        QDomText zoomFactorText = domDoc.createTextNode(QString::number(zoomFactor()));
        zoomFactorElement.appendChild(zoomFactorText);

        QDomElement audioDevicesElement = domDoc.createElement("audio_devices");
        rootElement.appendChild(audioDevicesElement);
        m_audioDevicesModel->appendItemsToDomElement(audioDevicesElement, domDoc);

        QDomElement videoDevicesElement = domDoc.createElement("video_devices");
        rootElement.appendChild(videoDevicesElement);
        m_videoDevicesModel->appendItemsToDomElement(videoDevicesElement, domDoc);

        QDomElement wanDevicesElement = domDoc.createElement("wan_devices");
        rootElement.appendChild(wanDevicesElement);
        m_wanDevicesModel->appendItemsToDomElement(wanDevicesElement, domDoc);

        QDomElement proxySettinsElement = m_proxySettings->toDomElement(domDoc);
        rootElement.appendChild(proxySettinsElement);

        QFile file (source().toLocalFile());
        if(file.open(QFile::WriteOnly)) {
            QTextStream ts(&file);
            ts.setCodec("UTF-8");
            ts << domDoc.toString();
            file.close();

            return true;
        }
    }

    return false;
}

QString Settings::emptyString() const
{
    return "";
}
