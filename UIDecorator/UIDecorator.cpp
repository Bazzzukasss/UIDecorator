#include "UIDecorator.h"
#include "ui_UIDecorator.h"
#include <QDir>
#include <QUiLoader>
#include <QTextStream>
#include <QDebug>
#include <QInputDialog>
#include <QClipboard>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>

UIDecorator::UIDecorator(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UIDecorator)
{
    ui->setupUi(this);
    initialize();
}

UIDecorator::~UIDecorator()
{
    saveSettings();
    delete ui;
}

void UIDecorator::initialize()
{
    mpLayout = new QVBoxLayout(ui->mUIFrame);
    mpLayout->setMargin(0);

    ui->mUITemplateFrame1->setAutoFillBackground(true);
    ui->mUITemplateFrame2->setAutoFillBackground(true);

    connect(ui->mComboBoxUITemplates,&QComboBox::currentTextChanged,this,[&](const QString& aFilename){ selectUITemplate(aFilename); });
    connect(ui->mComboBoxStyles,&QComboBox::currentTextChanged,     this,[&](const QString& aFilename){ selectStyle(aFilename); });
    connect(ui->mButtonNew,&QPushButton::clicked,                   this,[&](){ newStyle(); });
    connect(ui->mButtonDelete,&QPushButton::clicked,                this,[&](){ deleteStyle(ui->mComboBoxStyles->currentText()); });
    connect(ui->mButtonSave,&QPushButton::clicked,                  this,[&](){ saveStyle(ui->mComboBoxStyles->currentText()); });
    connect(ui->mButtonClipboard,&QPushButton::clicked,             this,[&](){ QApplication::clipboard()->setText(ui->mTextEdit->toPlainText()); });
    connect(ui->mTextEdit,&QTextEdit::textChanged,                  this,[&](){ applyStyle(); });
    connect(ui->mButtonOpenUI,&QPushButton::clicked,                this,[&](){ addUITemplate(); });

    initializeSettings();
    initializeDictionaries();
    refreshUITemplatesList();
    refreshStylesList();
    selectStyle(ui->mComboBoxStyles->currentText());
    selectUITemplate(ui->mComboBoxUITemplates->currentText());
}

void UIDecorator::saveSettings()
{
    mSettings->beginGroup("UITemplates");
    mSettings->beginWriteArray("files");
    int  i(0);
    for(auto& name : mUITemplates){
        mSettings->setArrayIndex(i++);
        mSettings->setValue("files",name);
    }
    mSettings->endArray();
    mSettings->endGroup();
}

void UIDecorator::initializeSettings()
{
    mSettings = new QSettings("config.ini",QSettings::IniFormat,this);
    mSettings->beginGroup("UITemplates");
    int size = mSettings->beginReadArray("files");
    for(int i = 0; i < size; i++){
        mSettings->setArrayIndex(i);
        mUITemplates.append( mSettings->value("files").toString());
    }
    mSettings->endArray();
    mSettings->endGroup();
}

void UIDecorator::initializeDictionaries()
{
    QTextCharFormat classesFormat,cssFormat,valuesFormat,controlsFormat,statesFormat;

    classesFormat.setForeground(Qt::red);
    classesFormat.setFontWeight(QFont::Bold);

    cssFormat.setForeground(Qt::blue);
    cssFormat.setFontWeight(QFont::Bold);

    valuesFormat.setForeground(Qt::gray);
    valuesFormat.setFontWeight(QFont::Bold);

    controlsFormat.setForeground(Qt::darkRed);
    controlsFormat.setFontWeight(QFont::Bold);

    statesFormat.setForeground(Qt::magenta);
    statesFormat.setFontWeight(QFont::Bold);

    initializeDictionaries("://resources/classes.txt","://resources/classes.png",classesFormat);
    initializeDictionaries("://resources/css.txt","://resources/css.png",cssFormat);
    initializeDictionaries("://resources/values.txt","://resources/values.png",valuesFormat);
    initializeDictionaries("://resources/sub-controls.txt","://resources/sub-controls.png",controlsFormat);
    initializeDictionaries("://resources/pseudo-states.txt","://resources/pseudo-states.png",statesFormat);
}

void UIDecorator::initializeDictionaries(const QString& aFilename, const QString aIcon, const QTextCharFormat& aFormat)
{    
    ui->mTextEdit->addCompletionDictionary(aFilename,aIcon);
    ui->mTextEdit->addHighlightionRule(aFilename,aFormat);
}

void UIDecorator::refreshStylesList(const QString &aFoldername)
{
    auto files = QDir(aFoldername).entryList(QStringList() << "*.css",QDir::Files);
    ui->mComboBoxStyles->blockSignals(true);
    ui->mComboBoxStyles->clear();
    ui->mComboBoxStyles->addItems(files);
    ui->mComboBoxStyles->blockSignals(false);
}

void UIDecorator::refreshUITemplatesList()
{
    ui->mComboBoxUITemplates->blockSignals(true);
    ui->mComboBoxUITemplates->clear();
    for(auto& filename : mUITemplates)
    {
        QFileInfo f(filename);
        if(f.exists())
            ui->mComboBoxUITemplates->addItem(filename);
    }
    ui->mComboBoxUITemplates->blockSignals(false);
}

void UIDecorator::selectUITemplate(const QString &aFilename)
{
    QUiLoader loader;
    QFile file(aFilename);
    file.open(QFile::ReadOnly);
    if(file.isOpen())
    {
        mpCurrentWidget = loader.load(&file,this);
        mpCurrentWidget->setAutoFillBackground(true);
        file.close();
        while(!mpLayout->isEmpty()) {
          delete mpLayout->takeAt(0);
        }
        mpLayout->addWidget(mpCurrentWidget);
        applyStyle();
    }
}

void UIDecorator::addUITemplate()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open UI template"), "", tr("UI Files (*.ui)"));

    if(fileName.isEmpty())
        return;

    if(!mUITemplates.contains(fileName))
    {
        mUITemplates.append(fileName);
        refreshUITemplatesList();
    }
    ui->mComboBoxUITemplates->blockSignals(true);
    ui->mComboBoxUITemplates->setCurrentText(fileName);
    ui->mComboBoxUITemplates->blockSignals(false);
    selectUITemplate(fileName);
}

void UIDecorator::loadStyle(const QString &aFilename)
{
    QFile file("Styles/"+aFilename);
    file.open(QFile::ReadOnly);
    if(file.isOpen())
    {
        QTextStream in(&file);
        ui->mTextEdit->setText(in.readAll());
        applyStyle();
        file.close();
    }
}

void UIDecorator::newStyle()
{
    bool ok;
    QString fileName = QInputDialog::getText(this, tr("New Style"),
                                             tr("Style name:"), QLineEdit::Normal,
                                             "StyleNew", &ok);
    if (!fileName.isEmpty())
    {
        saveStyle(fileName + ".css");        
        refreshStylesList();
        ui->mComboBoxStyles->blockSignals(true);
        ui->mComboBoxStyles->setCurrentText(fileName + ".css");
        ui->mComboBoxStyles->blockSignals(false);
    }
}

void UIDecorator::saveStyle(const QString& aFilename)
{
    QFile file("Styles/"+aFilename);

    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << ui->mTextEdit->toPlainText();
        file.close();
        ui->mTextEdit->setTextChangedState(false);
    }
}

void UIDecorator::deleteStyle(const QString &aFilename)
{
    QFile file("Styles/"+aFilename);
    auto res = QMessageBox::question(this, "UIDecorator", tr("Delete style: %1").arg(aFilename), QMessageBox::Yes|QMessageBox::No);
    if(res == QMessageBox::Yes)
        file.remove();
    refreshStylesList();
}

void UIDecorator::applyStyle()
{
    if(mpCurrentWidget!=nullptr)
        mpCurrentWidget->setStyleSheet( ui->mTextEdit->toPlainText() );
}

void UIDecorator::selectStyle(const QString& aFilename)
{
    if(ui->mTextEdit->isTextChanged())
    {
        auto res = QMessageBox::question(this, "UIDecorator", tr("The style %1 has been changed.Do you want to save changes?").arg(aFilename), QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

        if(res == QMessageBox::Yes)
        {
            saveStyle(mCurrentStyleName);
            ui->mTextEdit->setTextChangedState(false);
        }

        if(res == QMessageBox::No)
            ui->mTextEdit->setTextChangedState(false);

        if(res == QMessageBox::Cancel)
        {
            ui->mComboBoxStyles->blockSignals(true);
            ui->mComboBoxStyles->setCurrentText(mCurrentStyleName);
            ui->mComboBoxStyles->blockSignals(false);
            return;
        }
    }

    loadStyle(aFilename);
    mCurrentStyleName = aFilename;
}
