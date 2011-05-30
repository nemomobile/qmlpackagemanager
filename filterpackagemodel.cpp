#include "filterpackagemodel.h"

#include "packagemodel.h"


FilterPackageModel::FilterPackageModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

void FilterPackageModel::setSourceModel(QAbstractItemModel *sModel)
{
    PackageModel *packageModel = qobject_cast<PackageModel*>(sourceModel());

    if (packageModel) {
        disconnect(packageModel, SIGNAL(countChanged()), this, SIGNAL(totalCountChanged()));
        disconnect(packageModel, SIGNAL(markedCountChanged()), this, SIGNAL(markedCountChanged()));
        disconnect(packageModel, SIGNAL(countChanged()), this, SLOT(emitFilteredCountChanged()));
    }

    QSortFilterProxyModel::setSourceModel(sModel);

    packageModel = qobject_cast<PackageModel*>(sourceModel());

    if (packageModel) {
        connect(packageModel, SIGNAL(countChanged()), this, SIGNAL(totalCountChanged()));
        connect(packageModel, SIGNAL(markedCountChanged()), this, SIGNAL(markedCountChanged()));
        connect(packageModel, SIGNAL(countChanged()), this, SLOT(emitFilteredCountChanged()));
    }
}

void FilterPackageModel::mark(int row, bool set)
{
//    qDebug() << Q_FUNC_INFO << row << set;

    PackageModel *sModel = qobject_cast<PackageModel*>(sourceModel());
    sModel->mark(mapToSource(index(row,0)).row(), set);
}

void FilterPackageModel::resetMarkings()
{
    PackageModel *sModel = qobject_cast<PackageModel*>(sourceModel());
    sModel->resetMarkings();
}

void FilterPackageModel::setFilterString(const QString &text)
{
//    qDebug() << Q_FUNC_INFO << text;
    setFilterFixedString(text);
    emit filteredCountChanged();
}

int FilterPackageModel::filteredCount()
{
    return rowCount();
}

int FilterPackageModel::totalCount()
{
    return sourceModel()->rowCount();
}

int FilterPackageModel::markedCount()
{
    PackageModel *sModel = qobject_cast<PackageModel*>(sourceModel());
    sModel->markedCount();
}


void FilterPackageModel::emitFilteredCountChanged()
{
    QTimer::singleShot(50, this, SIGNAL(filteredCountChanged()));
}
