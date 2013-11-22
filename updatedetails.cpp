#include "updatedetails.h"
#include <QDebug>

//#define UPDATEDETAILS_LOG

UpdateDetails::UpdateDetails(const QString &packageID, const QStringList &updates, const QStringList &obsoletes, const QStringList &vendorUrls, const QStringList &bugzillaUrls, const QStringList &cveUrls, PackageKit::Transaction::Restart restart, const QString &updateText, const QString &changelog, PackageKit::Transaction::UpdateState state, const QDateTime &issued, const QDateTime &updated)
{
#ifdef UPDATEDETAILS_LOG
qDebug() << Q_FUNC_INFO;	
#endif

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

QStringList UpdateDetails::verndorUrls()
{
#ifdef UPDATEDETAILS_LOG
	    qDebug() << Q_FUNC_INFO;	
#endif	

    return m_vendorUrls;
}

QStringList UpdateDetails::bugzillaUrls()
{
#ifdef UPDATEDETAILS_LOG
qDebug() << Q_FUNC_INFO;	
#endif

    return m_bugzillaUrls;
}

QStringList UpdateDetails::cveUrls()
{
#ifdef UPDATEDETAILS_LOG
qDebug() << Q_FUNC_INFO;	
#endif

    return m_cveUrls;
}

PackageKit::Transaction::Restart UpdateDetails::restart()
{
#ifdef UPDATEDETAILS_LOG
qDebug() << Q_FUNC_INFO;	
#endif

    return m_restart;
}

QString UpdateDetails::updateText()
{
#ifdef UPDATEDETAILS_LOG
qDebug() << Q_FUNC_INFO;	
#endif

    return m_updateText;
}

QString UpdateDetails::changelog()
{
#ifdef UPDATEDETAILS_LOG
qDebug() << Q_FUNC_INFO;	
#endif

    return m_changelog;
}

PackageKit::Transaction::UpdateState UpdateDetails::state()
{
#ifdef UPDATEDETAILS_LOG
qDebug() << Q_FUNC_INFO;	
#endif
    return m_state;
}

QDateTime UpdateDetails::issued()
{
#ifdef UPDATEDETAILS_LOG
qDebug() << Q_FUNC_INFO;	
#endif
    return m_issued;
}

QDateTime UpdateDetails::updated()
{
#ifdef UPDATEDETAILS_LOG
qDebug() << Q_FUNC_INFO;	
#endif

    return m_updated;
}
