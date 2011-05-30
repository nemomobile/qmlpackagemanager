#include "iconprovider.h"

#include <QIcon>
#include <QFileInfo>
#include <QDebug>

IconProvider::IconProvider() :
    QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
}

QPixmap IconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
//    qDebug() << Q_FUNC_INFO << id << requestedSize;

    if (size)
        *size = QSize(32, 32);

    if (id.isEmpty())
        return QPixmap(1,1);

    QSize targetSize = requestedSize.isValid()? requestedSize: QSize(32, 32);

    QIcon icon = QIcon::fromTheme(id);
    if (icon.isNull())
        icon = QIcon(id);

    if (!icon.isNull()) {
        return icon.pixmap(targetSize);
    }

    return QPixmap(1,1);
}
