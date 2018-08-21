#ifndef RESOURCEDIALOG_H
#define RESOURCEDIALOG_H

#include <QDialog>
class QSettings;
class TreeItem;

namespace Ui {
class ResourceDialog;
}

class ResourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResourceDialog(QWidget *parent = 0);
    ~ResourceDialog();

    QString getResource();

private:
    QSettings* mpSettings;
    QString mResource;
    QString mResult;
    QStringList mResources;
    TreeItem* mTreeItem{nullptr};
    Ui::ResourceDialog *ui;

    void initialize();
    void openResourceFile();
    void selectResourceFile(const QString& aFilename);
    void showResources( const QStringList& aFiles, const QString& aCurrentFile );

};

#endif // RESOURCEDIALOG_H
