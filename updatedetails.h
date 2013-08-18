#ifndef UPDATEDETAILS_H
#define UPDATEDETAILS_H


#include <QString>
#include <QStringList>
#include <QDateTime>
#include "transaction.h"

class UpdateDetails
{
public:
    UpdateDetails(const QString &packageID,
                  const QStringList &updates,
                  const QStringList &obsoletes,
                  const QStringList &vendorUrls,
                  const QStringList &bugzillaUrls,
                  const QStringList &cveUrls,
                  PackageKit::Transaction::Restart restart,
                  const QString &updateText,
                  const QString &changelog,
                  PackageKit::Transaction::UpdateState state,
                  const QDateTime &issued,
                  const QDateTime &updated);
private:
    QString m_packageID;
    QStringList m_updates;
    QStringList m_obsoletes;
    QStringList m_vendorUrls;
    QStringList m_bugzillaUrls;
    QStringList m_cveUrls;
    PackageKit::Transaction::Restart m_restart;
    QString m_updateText;
    QString m_changelog;
    PackageKit::Transaction::UpdateState m_state;
    QDateTime m_issued;
    QDateTime m_updated;
};

#endif // UPDATEDETAILS_H
