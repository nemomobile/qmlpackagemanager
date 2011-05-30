#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QDeclarativeImageProvider>

class IconProvider : public QDeclarativeImageProvider
{
public:
    explicit IconProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

signals:

public slots:

};

#endif // ICONPROVIDER_H
