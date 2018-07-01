#ifndef GRADIENTDIALOG_H
#define GRADIENTDIALOG_H

#include <QDialog>

namespace Ui {
class GradientDialog;
}

class GradientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GradientDialog(QWidget *parent = 0);
    ~GradientDialog();

private:
    Ui::GradientDialog *ui;
};

#endif // GRADIENTDIALOG_H
