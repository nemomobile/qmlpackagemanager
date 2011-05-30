#ifndef FILTERPACKAGEMODEL_H
#define FILTERPACKAGEMODEL_H

#include <QSortFilterProxyModel>

class FilterPackageModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(int totalcount READ totalCount NOTIFY totalCountChanged)
    Q_PROPERTY(int filteredcount READ filteredCount NOTIFY filteredCountChanged)
    Q_PROPERTY(int markedcount READ markedCount NOTIFY markedCountChanged)

public:
    explicit FilterPackageModel(QObject *parent = 0);

    int totalCount();
    int filteredCount();
    int markedCount();

    void setSourceModel(QAbstractItemModel *sourceModel);

signals:
    void totalCountChanged();
    void filteredCountChanged();
    void markedCountChanged();

public slots:
    void mark(int row, bool set);
    void resetMarkings();
    void setFilterString(const QString &text);

private slots:
    void emitFilteredCountChanged();
};

#endif // FILTERPACKAGEMODEL_H
