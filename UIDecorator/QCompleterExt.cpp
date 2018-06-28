#include "QCompleterExt.h"
#include <QFile>
#include <set>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTableView>

QCompleterExt::QCompleterExt(QObject *parent)
    :QCompleter(parent)
{
    initialize();
}

QCompleterExt::~QCompleterExt()
{
    delete mModel;
}

void QCompleterExt::initialize()
{
    mView = new QTableView(qobject_cast<QWidget*>(this));
    mView->setAlternatingRowColors(true);
    mView->setShowGrid(false);
    mView->horizontalHeader()->setStretchLastSection(true);
    mView->horizontalHeader()->hide();
    mView->verticalHeader()->hide();
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mView->setWordWrap(true);

    setPopup(mView);
}

void QCompleterExt::addDictionary(const QString &aFilename, const QString &aIconFilename)
{
    QFile file(aFilename);
    if (file.open(QFile::ReadOnly))
    {
        QStringList dataList;

        while(!file.atEnd())
        dataList.append(file.readLine());

        mDictionaries.push_back( {aIconFilename,dataList} );
        resetModel();

        mView->resizeColumnsToContents();
        mView->sortByColumn(0);
    }    
}

void QCompleterExt::clearDictionaries()
{
    mDictionaries.clear();
}

void QCompleterExt::resetModel()
{
    if(mModel != nullptr)
        delete mModel;

    mModel = new QStandardItemModel(0,2);

    for(auto& dictionary : mDictionaries)
        for(auto& data : dictionary.mData)
        {
            QList<QStandardItem*> newRow;

            QIcon icon(dictionary.mIcon);
            int delimerPos = data.indexOf("#");
            QString tag = delimerPos > 0 ? data.left(delimerPos): data;
            QString description = delimerPos > 0 ? data.right(data.length() - delimerPos - 1) : "";

            QStandardItem* itemTag = new QStandardItem( icon, tag );
            QStandardItem* itemDescription = new QStandardItem( description );
            newRow.append({itemTag,itemDescription});

            mModel->appendRow(newRow);
        }

    setModel( mModel );
}
