#ifndef QCOMPLETEREXT_H
#define QCOMPLETEREXT_H

#include <QCompleter>

class QStandardItemModel;

struct CompletionDictionary{
    QString mIcon;
    QStringList mWords;
};

class QCompleterExt : public QCompleter
{
    Q_OBJECT
public:
    QCompleterExt(QObject* parent = 0);
    ~QCompleterExt();

    void addDictionary(const QString& aFilename,const QString& aIconFilename = "");
    void clearDictionaries();

private:
    QStandardItemModel* mModel{nullptr};
    QVector<CompletionDictionary> mDictionaries;

    void resetModel();
    void resetCompleter();
};

#endif // QCOMPLETEREXT_H
