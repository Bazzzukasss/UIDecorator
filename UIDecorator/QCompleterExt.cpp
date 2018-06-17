#include "QCompleterExt.h"
#include <QFile>
#include <set>
#include <QModelIndex>
#include <QStandardItemModel>

QCompleterExt::QCompleterExt(QObject *parent)
    :QCompleter(parent)
{    
}

QCompleterExt::~QCompleterExt()
{
    delete mModel;
}

void QCompleterExt::addDictionary(const QString &aFilename, const QString &aIconFilename)
{
    QFile file(aFilename);
    if (file.open(QFile::ReadOnly))
    {
        QStringList wordsList;
        std::set<std::string> wordsSet;

        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            if (!line.isEmpty())
            {
                QStringList wordsInLine = QString(line.trimmed()).split(":");
                wordsSet.insert(wordsInLine.at(0).toStdString());
            }
        }
        for(auto& str : wordsSet)
            wordsList << QString().fromStdString(str);

        mDictionaries.push_back( {aIconFilename,wordsList} );
        resetModel();
    }
}

void QCompleterExt::clearDictionaries()
{
    mDictionaries.clear();
}

void QCompleterExt::resetModel()
{
    std::set< std::pair<std::string,std::string> > wordsSet;

    if(mModel != nullptr)
        delete mModel;

    for(auto& dictionary : mDictionaries)
        for(auto& word : dictionary.mWords)
            wordsSet.insert(std::pair<std::string,std::string>(word.toStdString(),dictionary.mIcon.toStdString()));

    mModel = new QStandardItemModel(0,1);

    for(auto& word : wordsSet)
    {
        QList<QStandardItem*> newRow;
        for (int i=0;i<mModel->columnCount();i++)
        {
            QIcon icon(QString().fromStdString(word.second));
            QString string = QString().fromStdString(word.first);

            QStandardItem* itm = new QStandardItem( icon, string);
            newRow.append(itm);
        }
        mModel->appendRow(newRow);
    }

    setModel( mModel );
}
