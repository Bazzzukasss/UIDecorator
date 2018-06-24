#ifndef QCOMPLETEREXT_H
#define QCOMPLETEREXT_H

#include <QCompleter>
class QTableView;

class QStandardItemModel;

struct CompletionDictionary{
    QString mIcon;
    QStringList mData;
};

class QCompleterExt : public QCompleter
{
    Q_OBJECT
public:
    QCompleterExt(QObject *parent = 0);
    ~QCompleterExt();

    void addDictionary(const QString& aFilename,const QString& aIconFilename = "");
    void clearDictionaries();

private:
    QTableView* mView;
    QStandardItemModel* mModel{nullptr};
    QVector<CompletionDictionary> mDictionaries;

    void resetModel();
    void initialize();
};

#endif // QCOMPLETEREXT_H
