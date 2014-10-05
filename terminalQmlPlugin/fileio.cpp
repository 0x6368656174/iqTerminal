#include "fileio.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

FileIO::FileIO(QObject *parent) :
    QObject(parent)
{
}

QUrl FileIO::source() const
{
    return m_source;
}

void FileIO::setSource(const QUrl &source)
{
    if (m_source != source) {
        if (QFile::exists(source.toLocalFile())) {
            m_source = source;
        } else {
            emit error(tr("File not found."));
            qWarning() << tr("Not found %0.").arg(source.toString());
            m_source.clear();
        }

        emit sourceChanged();
    }
}

QString FileIO::read()
{
    if (m_source.isEmpty()) {
        emit error(tr("Source is empty."));
        qWarning() << tr("Source is empty.");
        return QString();
    }

    QFile file(m_source.toLocalFile());
    QString fileContent;
    if ( file.open(QIODevice::ReadOnly) ) {
        QString line;
        QTextStream t( &file );
        do {
            line = t.readLine();
            fileContent += line;
        } while (!line.isNull());

        file.close();
    } else {
        emit error(tr("Unable to open the file %0.").arg(m_source.toLocalFile()));
        qWarning() <<tr ("Unable to open the file %0.").arg(m_source.toLocalFile());
        return QString();
    }
    return fileContent;
}

bool FileIO::write(const QString& data)
{
    if (m_source.isEmpty()) {
        emit error(tr("Source is empty."));
        qWarning() << tr("Source is empty.");
        return false;
    }

    QFile file(m_source.toLocalFile());
    if (!file.open(QFile::WriteOnly)) {
        QFileInfo info (m_source.toLocalFile());
        qDebug() << info.exists();
        qDebug() << info.size();

        emit error(tr("Unable to open the file %0.").arg(m_source.toLocalFile()));
        qWarning() << tr("Unable to open the file %0.").arg(m_source.toLocalFile());
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << data;

    file.close();

    return true;
}
