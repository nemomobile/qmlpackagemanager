#ifndef TRANSACTIONWRAPPER_H
#define TRANSACTIONWRAPPER_H

#include <QObject>

#include <QPackageKit>

class TransactionWrapper : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString state READ state NOTIFY changed);
    Q_PROPERTY(uint errorCode READ errorCode NOTIFY changed);
    Q_PROPERTY(QString errorText READ errorText NOTIFY changed);
    Q_PROPERTY(bool allowCancel READ allowCancel NOTIFY changed);
    Q_PROPERTY(int percentage READ percentage NOTIFY changed);
    Q_PROPERTY(int subpercentage READ subpercentage NOTIFY changed);
    Q_PROPERTY(uint status READ status NOTIFY changed);
    Q_PROPERTY(QString currentPackage READ currentPackage NOTIFY changed);

public:
    explicit TransactionWrapper(PackageKit::Transaction *transaction, bool readPackages = true, QObject *parent = 0);

    QString state();
    uint errorCode();
    QString errorText();
    bool allowCancel();
    int percentage();
    int subpercentage();
    uint status();
    QString currentPackage();

signals:
    void changed();

private slots:
    void onChanged();
    void onPackage(QSharedPointer<PackageKit::Package> packagePtr);
    void onErrorCode(PackageKit::Enum::Error error, const QString& details);
    void onMessage(PackageKit::Enum::Message type, const QString &message);
    void onRepoSignatureRequired(PackageKit::Client::SignatureInfo info);
    void onFinished(PackageKit::Enum::Exit status,uint runtime);
    void onDestroyed();

private:
    PackageKit::Transaction *m_transaction;

    QString m_errorText;
    PackageKit::Enum::Error m_errorCode;

    QString m_state;

    QString m_currentPackage;
};

#endif // TRANSACTIONWRAPPER_H
