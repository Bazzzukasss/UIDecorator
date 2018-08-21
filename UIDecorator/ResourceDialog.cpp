#include "ResourceDialog.h"
#include "ui_ResourceDialog.h"
#include <QFileDialog>
#include <QSettings>
#include "Treeitem.h"
#include "XMLProcessor.h"
#include "UIRoutiner.h"

ResourceDialog::ResourceDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::ResourceDialog)
{
    ui->setupUi(this);
    initialize();
}

ResourceDialog::~ResourceDialog()
{
    delete mTreeItem;
    delete ui;
}

QString ResourceDialog::getResource()
{
    exec();
    QString res = mResult;
    mResult = "";
    return res;
}

void ResourceDialog::initialize()
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    connect(ui->mComboBoxResources,&QComboBox::currentTextChanged, this,[&](const QString& aFilename){ pROUTINER->selectResource(aFilename); });
    connect(ui->mButtonOk,&QPushButton::clicked,                        [&](){ mResult = mResource;close(); });
    connect(ui->mButtonCancel,&QPushButton::clicked,                    [&](){ close(); });
    connect(ui->mButtonOpen, &QPushButton::clicked, this,               [&](){ openResourceFile(); });

    connect(pROUTINER,&UIRoutiner::signalResourcesListChanged, this,    [&](const QStringList& aFiles, const QString& aCurrentFile){ showResources(aFiles,aCurrentFile); });

}

void ResourceDialog::showResources( const QStringList& aFiles, const QString& aCurrentFile )
{
    static QString prevFile;

    ui->mComboBoxResources->blockSignals(true);
    ui->mComboBoxResources->clear();
    ui->mComboBoxResources->addItems(aFiles);
    ui->mComboBoxResources->setCurrentText(aCurrentFile);
    ui->mComboBoxResources->blockSignals(false);

    if(prevFile != aCurrentFile)
    {
        prevFile = aCurrentFile;
        selectResourceFile(aCurrentFile);
    }
}

void ResourceDialog::openResourceFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Resource"), "", tr("Resource Files (*.qrc)"));

    if(!fileName.isEmpty())
        pROUTINER->addRecource(fileName);
}

void ResourceDialog::selectResourceFile(const QString &aFilename)
{
    QFile file(aFilename);
    file.open(QFile::ReadOnly);
    if(file.isOpen())
    {
        if(mTreeItem != nullptr)
            delete mTreeItem;
        mTreeItem = new TreeItem({});
        XMLProcessor::Load(aFilename,mTreeItem);
    }
}

