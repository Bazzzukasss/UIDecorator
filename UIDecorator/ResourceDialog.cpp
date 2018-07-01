#include "ResourceDialog.h"
#include "ui_ResourceDialog.h"

ResourceDialog::ResourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResourceDialog)
{
    ui->setupUi(this);
    initialize();
}

ResourceDialog::~ResourceDialog()
{
    delete ui;
}

QString ResourceDialog::getResource()
{
    exec();
    mResult = "TEST";
    return mResult;
}

void ResourceDialog::initialize()
{
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
}

