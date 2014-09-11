#include "filewatcher.h"
#include <QFileInfo>
#include <QDir>

FileWatcher::FileWatcher(QObject *parent) :
    QFileSystemWatcher(parent),
    m_existed(false)
{
    connect(this, SIGNAL(directoryChanged(QString)), this, SLOT(checkIsFileExist()));
}

QString FileWatcher::file() const
{
    return m_file;
}

void FileWatcher::setFile(const QString &file)
{
    if (m_file != file) {
        m_file = file;
        emit fileChanged();

        QFileInfo fileInfo (file);
        if (!directories().isEmpty())
            removePaths(directories());
        addPath(fileInfo.absoluteDir().absolutePath());
    }
}

bool FileWatcher::exist()
{
    return QFileInfo::exists(m_file);
}

void FileWatcher::checkIsFileExist()
{
    if (QFileInfo::exists(m_file)) {
        if (!m_existed)
            emit created();
        m_existed = true;
    } else {
        if (m_existed)
            emit deleted();
        m_existed = false;
    }
}
