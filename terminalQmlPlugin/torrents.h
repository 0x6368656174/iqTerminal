#ifndef TORRENTS_H
#define TORRENTS_H

#include <QObject>
#include "torrentfoldersmodel.h"

/*!
 * \brief Класс, содержащий модели для торрента
 *
 * Данный класс содержит модели для торрента и используется в QML как источник данных для транички "Торрент".
 *
 * \ingroup AppModel
 */
class Torrents : public QObject
{
    Q_OBJECT
    /*!
     * \brief Указатель на модель поиска торрента
     *
     * \accessors %searchModel()
     */
    Q_PROPERTY(TorrentFoldersModel* searchModel READ searchModel CONSTANT)
    /*!
     * \brief Указатель на модель загрузки торрента
     *
     * \accessors %downloadModel()
     */
    Q_PROPERTY(TorrentFoldersModel* downloadModel READ downloadModel CONSTANT)
    /*!
     * \brief Указатель на модель раздачи торрента
     *
     * \accessors %uploadModel()
     */
    Q_PROPERTY(TorrentFoldersModel* uploadModel READ uploadModel CONSTANT)
public:
    /*!
     * \brief Конструктор
     *
     * Создает экземпляр объекта с указанным родителем.
     *
     * \param parent Укзатель на родитель
     */
    explicit Torrents(QObject *parent = 0);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
public:
    TorrentFoldersModel *searchModel() const;

    TorrentFoldersModel *downloadModel() const;

    TorrentFoldersModel *uploadModel() const;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

private:
    TorrentFoldersModel *m_searchModel;
    TorrentFoldersModel *m_downloadModel;
    TorrentFoldersModel *m_uploadModel;
};

#endif // TORRENTS_H
