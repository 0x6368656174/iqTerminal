#include "textdecorator.h"
#include <QDomDocument>
#include <QUrl>
#include <QDebug>
#include "core.h"

QHash<QString, QString> TextDecorator::m_smiles = QHash<QString, QString>();

TextDecorator::TextDecorator(QObject *parent) :
    QObject(parent)
{
}

void TextDecorator::initSmiles()
{
    if (!m_smiles.isEmpty())
        return;

    m_smiles[":-)"] = "1a.png";
}

QString TextDecorator::toPlainText(const QString &str)
{
    initSmiles();

    QDomDocument domDoc;
    if (!domDoc.setContent(str))
        return "";

    QDomElement bodyElement = domDoc.documentElement().firstChildElement("body");
    if (bodyElement.isNull())
        return "";

    QString result;

    QDomElement pElement = bodyElement.firstChildElement("p");
    while (!pElement.isNull()) {
        QDomNode childNode = pElement.firstChild();
        while (!childNode.isNull()) {
            if (childNode.isElement()) {
                QDomElement childElement = childNode.toElement();
                if (childElement.tagName() == "span") {
                    result += childElement.text();
                } else if (childElement.tagName() == "img") {
                    QString src = childElement.attribute("src");
                    QUrl srcUrl (src);
                    QString smile = m_smiles.key(srcUrl.fileName());
                    if (smile.isEmpty()) {
                        QString whiteSmile = srcUrl.fileName();
                        whiteSmile.insert(whiteSmile.length() - 4, "a");
                        smile = m_smiles.key(whiteSmile);
                    }
                    result += smile;
                }
            } else if (childNode.isText()) {
                QDomText childText = childNode.toText();
                result += childText.data();
            }

            childNode = childNode.nextSibling();
        }

        result += "\n";

        pElement = pElement.nextSiblingElement();
    }

    return result.trimmed();
}

QString TextDecorator::toFormattedText(const QString &str)
{
    initSmiles();

    QString result;
    QStringList rows = str.split("\n");

    foreach (QString row, rows) {
        QString formatedRow = row;
        QHashIterator<QString, QString> smileIterator (m_smiles);
        while (smileIterator.hasNext()) {
            smileIterator.next();
            formatedRow.replace(smileIterator.key(), "<img src=\""+ Core::dataDir().toString() +"smiles/" + smileIterator.value() +
                                "\" width=\"" + QString::number(Core::dp(8)) +
                                "\" height=\""+ QString::number(Core::dp(8)) +"\" />");
        }
        result += formatedRow + "<br>";
    }

    result.remove(result.length() - 4, 4);

    return result;
}
