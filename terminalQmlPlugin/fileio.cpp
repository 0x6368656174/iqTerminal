#include "fileio.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

FileIO::FileIO(QObject *parent) :
    QObject(parent)
{
}

void FileIO::setSource(const QUrl &source)
{
    if (_source != source)
    {
        if (QFile::exists(source.toLocalFile()))
        {
            _source = source;
        }
        else
        {
            emit error(tr("File not found."));
            qWarning() << tr("Not found %0.").arg(source.toString());
            _source.clear();
        }

        emit sourceChanged();
    }
}

QString FileIO::read()
{
    if (_source.isEmpty())
    {
        emit error(tr("Source is empty."));
        qWarning() << tr("Source is empty.");
        return QString();
    }

    QFile file(_source.toLocalFile());
    QString fileContent;
    if ( file.open(QIODevice::ReadOnly) )
    {
        QString line;
        QTextStream t( &file );
        do
        {
            line = t.readLine();
            fileContent += line;
        }
        while (!line.isNull());

        file.close();
    }
    else
    {
        emit error(tr("Unable to open the file %0.").arg(_source.toLocalFile()));
        qWarning() <<tr ("Unable to open the file %0.").arg(_source.toLocalFile());
        return QString();
    }
    return fileContent;
}

bool FileIO::write(const QString& data)
{
    if (_source.isEmpty())
    {
        emit error(tr("Source is empty."));
        qWarning() << tr("Source is empty.");
        return false;
    }

    QFile file(_source.toLocalFile());
    if (!file.open(QFile::WriteOnly))
    {
        QFileInfo info (_source.toLocalFile());
        qDebug() << info.exists();
        qDebug() << info.size();

        emit error(tr("Unable to open the file %0.").arg(_source.toLocalFile()));
        qWarning() << tr("Unable to open the file %0.").arg(_source.toLocalFile());
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << data;

    file.close();

    return true;
}
