#include "updatedetails.h"

UpdateDetails::UpdateDetails(const QString &packageID, const QStringList &updates, const QStringList &obsoletes, const QStringList &vendorUrls, const QStringList &bugzillaUrls, const QStringList &cveUrls, PackageKit::Transaction::Restart restart, const QString &updateText, const QString &changelog, PackageKit::Transaction::UpdateState state, const QDateTime &issued, const QDateTime &updated)
{
    m_packageID = packageID;
    m_updates = updates;
    m_obsoletes = obsoletes;
    m_vendorUrls = vendorUrls;
    m_bugzillaUrls = bugzillaUrls;
    m_cveUrls = cveUrls;
    m_restart = restart;
    m_updateText = updateText;
    m_changelog = changelog;
    m_state = state;
    m_issued = issued;
    m_updated = updated;
}
