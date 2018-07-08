#include "ResourceDialog.h"
#include "ui_ResourceDialog.h"
#include <QFileDialog>
#include <QSettings>
#include "Treeitem.h"
#include "XMLProcessor.h"

ResourceDialog::ResourceDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::ResourceDialog)
{
    ui->setupUi(this);
    initialize();
}

ResourceDialog::~ResourceDialog()
{
    saveSettings();
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

    mpSettings = new QSettings("config.ini",QSettings::IniFormat,this);


    connect(ui->mButtonOk,&QPushButton::clicked,                                                        [&](){ mResult = mResource;close(); });
    connect(ui->mButtonCancel,&QPushButton::clicked,                                                    [&](){ close(); });
    connect(ui->mButtonOpen, &QPushButton::clicked, this, [&](){ addRecourceFile(); });
    connect(ui->mComboBoxResources,&QComboBox::currentTextChanged,this,[&](const QString& aFilename){ selectResourceFile(aFilename); });

    loadSettings();
    refreshResourcesList();
    selectResourceFile(ui->mComboBoxResources->currentText());
}

void ResourceDialog::loadSettings()
{
    mResources.clear();
    mpSettings->beginGroup("Resources");
    int size = mpSettings->beginReadArray("files");
    for(int i = 0; i < size; i++){
        mpSettings->setArrayIndex(i);
        mResources.append( mpSettings->value("files").toString());
    }
    mpSettings->endArray();
    mpSettings->endGroup();
}

void ResourceDialog::saveSettings()
{
    mpSettings->beginGroup("Resources");
    mpSettings->beginWriteArray("files");
    int  i(0);
    for(auto& name : mResources){
        mpSettings->setArrayIndex(i++);
        mpSettings->setValue("files",name);
    }
    mpSettings->endArray();
    mpSettings->endGroup();
}

void ResourceDialog::refreshResourcesList()
{
    ui->mComboBoxResources->blockSignals(true);
    ui->mComboBoxResources->clear();
    for(auto& filename : mResources)
    {
        QFileInfo f(filename);
        if(f.exists())
            ui->mComboBoxResources->addItem(filename);
    }
    ui->mComboBoxResources->blockSignals(false);
}

void ResourceDialog::addRecourceFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Resource"), "", tr("Resource Files (*.qrc)"));

    if(fileName.isEmpty())
        return;

    if(!mResources.contains(fileName))
    {
        mResources.append(fileName);
        refreshResourcesList();
    }
    ui->mComboBoxResources->blockSignals(true);
    ui->mComboBoxResources->setCurrentText(fileName);
    ui->mComboBoxResources->blockSignals(false);
    selectResourceFile(fileName);
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

