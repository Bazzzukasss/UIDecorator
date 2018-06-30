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

void ResourceDialog::initialize()
{
    connect(ui->mB)
}
