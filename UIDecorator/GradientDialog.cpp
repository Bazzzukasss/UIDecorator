#include "GradientDialog.h"
#include "ui_GradientDialog.h"

GradientDialog::GradientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradientDialog)
{
    ui->setupUi(this);
}

GradientDialog::~GradientDialog()
{
    delete ui;
}
