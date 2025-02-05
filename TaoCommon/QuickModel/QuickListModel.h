﻿#pragma once

#include "QuickListItemBase.h"
#include "QuickModelBase.hpp"
#include "TaoCommonGlobal.h"
#include <QTimer>
class TAO_API QuickListModel : public QuickModelBase<QuickListItemBase *>
{
    Q_OBJECT
    Q_PROPERTY(bool allChecked READ allChecked WRITE setAllChecked NOTIFY allCheckedChanged)
    Q_PROPERTY(int visibledCount READ visibledCount NOTIFY visibledCountChanged)
    Q_PROPERTY(int selectedCount READ selectedCount NOTIFY selectedCountChanged)
    Q_PROPERTY(int checkedCount READ checkedCount NOTIFY checkedCountChanged)

    Q_PROPERTY(QStringList headerRoles READ headerRoles WRITE setHeaderRoles NOTIFY headerRolesChanged)
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY sortOrderChanged)
    Q_PROPERTY(QString sortRole READ sortRole WRITE setSortRole NOTIFY sortRoleChanged)

    Q_PROPERTY(QStringList tableHeaders READ tableHeaders WRITE setTableHeaders NOTIFY tableHeadersChanged)
public:
    using Super = QuickModelBase<QuickListItemBase *>;
    explicit QuickListModel(QObject *parent = nullptr);
    ~QuickListModel() override;
    Q_INVOKABLE QVariant data(int row) const
    {
        return Super::data(row);
    }
    //[begin] check
    bool allChecked() const
    {
        return mAllChecked;
    }
    Q_INVOKABLE void check(int row, bool checked);
    //[end] check

    //[begin] search. control visible
    Q_INVOKABLE void search(const QString &searchKey);
    const QString &  searchKey() const
    {
        return mSearchkey;
    }
    Q_INVOKABLE void searchImmediate(const QString &searchKey);
    Q_INVOKABLE void clearSearchKey()
    {
        mSearchkey.clear();
    }
    //[end] search

    using VisibleCallback = std::function<bool(QuickListItemBase *)>;
    void setVisibleFilter(const VisibleCallback &callback)
    {
        mVisibleCallback = callback;
    }

    //[begin] select
    Q_INVOKABLE void deselectAll();
    Q_INVOKABLE void selectAll();
    Q_INVOKABLE bool isSelected(int row) const;
    Q_INVOKABLE void select(int row);
    Q_INVOKABLE void deselect(int row);
    Q_INVOKABLE void selectRange(int from, int to);
    Q_INVOKABLE void selectSingle(int row);
    //[end] select

    Q_INVOKABLE void doPress(int row, bool shift, bool ctrl, bool outRange);
    Q_INVOKABLE void doMove(int row, bool outRange);
    Q_INVOKABLE void doRelease();

    const QStringList &tableHeaders() const
    {
        return mTableHeaders;
    }
    //[begin] sort
    const QStringList &headerRoles() const
    {
        return mHeaderRoles;
    }

    Qt::SortOrder sortOrder() const
    {
        return mSortOrder;
    }

    const QString &sortRole() const
    {
        return mSortRole;
    }
    using SortCallback = std::function<bool(QuickListItemBase *, QuickListItemBase *)>;
    // Map <key, callBack> ,key should match to headerRoles
    void setSortCallbacksAscend(const QMap<QString, SortCallback> &callbacksMap)
    {
        mSortCallbacksAscend = callbacksMap;
    }
    void setSortCallbacksDescend(const QMap<QString, SortCallback> &callbacksMap)
    {
        mSortCallbacksDescend = callbacksMap;
    }
    
    Q_INVOKABLE virtual void sortByRole();
    //[end] sort

    //[begin] count
    int visibledCount() const
    {
        return mVisibledCount;
    }

    int selectedCount() const
    {
        return mSelectedCount;
    }

    int checkedCount() const
    {
        return mCheckedCount;
    }
    //[end] count

    void updateCalcInfo() override;

    Q_INVOKABLE void notifyScrollTo(int index)
    {
        emit scrollTo(index);
    }
public slots:
    void setAllChecked(bool allChecked);

    void setTableHeaders(const QStringList &tableHeaders);
    void setHeaderRoles(const QStringList &headerRoles);

    void setSortOrder(Qt::SortOrder sortOrder);

    void setSortRole(const QString &sortRole);

    void setVisibledCount(int visibledCount);
    void setSelectedCount(int selectedCount);
    void setCheckedCount(int checkedCount);


signals:
    void scrollTo(int index);
    void allCheckedChanged(bool allChecked);
    void visibledCountChanged(int visibledCount);

    void selectedAction();

    void selectedCountChanged(int selectedCount);

    void checkedCountChanged(int checkedCount);

    void headerRolesChanged(const QStringList &headerRoles);

    void sortOrderChanged(Qt::SortOrder sortOrder);

    void sortRoleChanged(const QString &sortRole);

    void signalUpdateCalcCount();
    void tableHeadersChanged(const QStringList &tableHeaders);

    void beginSearch();
    void endSearch();
protected slots:
    void onSearch();

protected:
    void updateAllCheck();
    void updateVisibleCount();
    void updateSelectedCount();
    void updateCheckedCount();
    void updateAlternate();

protected:
    bool                        mAllChecked = false;
    bool                        mIsPressed = false;
    Qt::SortOrder               mSortOrder = Qt::AscendingOrder;
    int                         mVisibledCount = 0;
    int                         mSelectedCount = 0;
    int                         mCheckedCount = 0;
    int                         mLastPressedRow = -1;
    QStringList                 mTableHeaders;
    QStringList                 mHeaderRoles;
    QString                     mSortRole;
    QMap<QString, SortCallback> mSortCallbacksAscend;
    QMap<QString, SortCallback> mSortCallbacksDescend;
    QString                     mSearchkey;
    QTimer                      mSearchTimer;
    VisibleCallback             mVisibleCallback;
};
