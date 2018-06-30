#ifndef RESOURCEDIALOG_H
#define RESOURCEDIALOG_H

#include <QDialog>

namespace Ui {
class ResourceDialog;
}

class ResourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResourceDialog(QWidget *parent = 0);
    ~ResourceDialog();

private:
    Ui::ResourceDialog *ui;

    void initialize();
};

#endif // RESOURCEDIALOG_H
