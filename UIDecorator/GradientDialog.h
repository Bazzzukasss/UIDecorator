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

    QString getGradient();

private:
    QString mResult;
    QString mGradient;
    QVector<QColor> mColors;
    Ui::GradientDialog *ui;

    void initialize();
    void selectColor(int aIndex);
    void switchColors();
    void drawGradient();
};

#endif // GRADIENTDIALOG_H
