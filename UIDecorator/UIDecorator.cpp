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
#include <QColorDialog>
#include <QFontDialog>
#include <QMenu>
#include "GradientDialog.h"
#include "UIRoutiner.h"

#define STYLES_FOLDER "Styles"

UIDecorator::UIDecorator(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UIDecorator)
{
    ui->setupUi(this);
    initialize();
}

UIDecorator::~UIDecorator()
{
    delete ui;
}

void UIDecorator::initialize()
{
    mpLayout = new QVBoxLayout(ui->mUIFrame);
    mpLayout->setMargin(0);
    mpGradientDialog = new GradientDialog(this);

    ui->mUITemplateFrame1->setAutoFillBackground(true);
    ui->mUITemplateFrame2->setAutoFillBackground(true);
    ui->mComboBoxUITemplates->view()->setTextElideMode(Qt::ElideLeft);
    ui->mComboBoxStyles->view()->setTextElideMode(Qt::ElideLeft);

    connect(ui->mComboBoxUITemplates,&QComboBox::currentTextChanged,this,[&](const QString& aFilename){ pROUTINER->selectUITemplate(aFilename); });
    connect(ui->mComboBoxStyles,&QComboBox::currentTextChanged,     this,[&](const QString& aFilename){
                                                                                                            QString name = checkStyle() ? aFilename : "";
                                                                                                            pROUTINER->selectStyle(name);
                                                                                                        });

    connect(ui->mButtonNew,&QPushButton::clicked,                   this,[&](){ newStyle(); });
    connect(ui->mButtonOpenStyle,&QPushButton::clicked,             this,[&](){ openStyle(); });
    connect(ui->mButtonDelete,&QPushButton::clicked,                this,[&](){ deleteStyle(); });
    connect(ui->mButtonSave,&QPushButton::clicked,                  this,[&](){ saveStyle(); });
    connect(ui->mButtonClipboard,&QPushButton::clicked,             this,[&](){ QApplication::clipboard()->setText(ui->mTextEdit->toPlainText()); });
    connect(ui->mTextEdit,&QTextEdit::textChanged,                  this,[&](){ applyStyle(); ui->mTextEdit->setTextChangedState(true); });
    connect(ui->mButtonOpenUI,&QPushButton::clicked,                this,[&](){ openUITemplateFile(); });
    connect(ui->mButtonFont,&QPushButton::clicked,                  this,[&](){ insertFont(); });

    connect(pROUTINER,&UIRoutiner::signalStylesListChanged,          this,[&](const QStringList& aFiles, const QString& aCurrentFile){ showStyles(aFiles,aCurrentFile); });
    connect(pROUTINER,&UIRoutiner::signalUITemplatesListChanged,     this,[&](const QStringList& aFiles, const QString& aCurrentFile){ showUITemplates(aFiles,aCurrentFile); });

    //Color Menu
    QMenu* mColorSelectionMenu = new QMenu(this);
    ui->mButtonColor->setMenu(mColorSelectionMenu);
    QStringList colorProperties{"color","background-color","alternate-background-color","border-color","border-top-color","border-right-color",
                               "border-bottom-color","border-left-color","gridline-color","selection-color","selection-background-color"};

    for(auto& property : colorProperties)
        mColorSelectionMenu->addAction(property,[=](){insertColor(property);});

    //Resource Menu
    QMenu* mResourceSelectionMenu = new QMenu(this);
    ui->mButtonResource->setMenu(mResourceSelectionMenu);
    QStringList resourceProperties{"image","background-image","border-image"};

    for(auto& property : resourceProperties)
        mResourceSelectionMenu->addAction(property,[=](){insertResource(property);});

    //Gradient Menu
    QMenu* mGradientSelectionMenu = new QMenu(this);
    ui->mButtonGradient->setMenu(mGradientSelectionMenu);

    for(auto& property : colorProperties)
        mGradientSelectionMenu->addAction(property,[=](){insertGradient(property);});

    initializeDictionaries();

    pROUTINER->initialize();
}

void UIDecorator::initializeDictionaries()
{
    QTextCharFormat classesFormat,cssFormat,valuesFormat,controlsFormat,statesFormat;

    classesFormat.setForeground(Qt::red);
    classesFormat.setFontWeight(QFont::Bold);

    cssFormat.setForeground(Qt::blue);
    cssFormat.setFontWeight(QFont::Bold);

    valuesFormat.setForeground(Qt::darkGray);
    valuesFormat.setFontWeight(QFont::Bold);

    controlsFormat.setForeground(Qt::darkRed);
    controlsFormat.setFontWeight(QFont::Bold);

    statesFormat.setForeground(Qt::magenta);
    statesFormat.setFontWeight(QFont::Bold);

    //initializeDictionaries("://resources/values.txt","://resources/values.png",valuesFormat);
    initializeDictionaries("://resources/classes.txt","://resources/classes.png",classesFormat);
    initializeDictionaries("://resources/css.txt","://resources/css.png",cssFormat);
    initializeDictionaries("://resources/sub-controls.txt","://resources/sub-controls.png",controlsFormat);
    initializeDictionaries("://resources/pseudo-states.txt","://resources/pseudo-states.png",statesFormat);
}

void UIDecorator::initializeDictionaries(const QString& aFilename, const QString aIcon, const QTextCharFormat& aFormat)
{    
    ui->mTextEdit->addCompletionDictionary(aFilename,aIcon);
    ui->mTextEdit->addHighlightionRule(aFilename,aFormat);
}

void UIDecorator::showStyles( const QStringList& aFiles, const QString& aCurrentFile )
{
    static QString prevFile;
    showFiles(ui->mComboBoxStyles,aFiles,aCurrentFile);
    if(prevFile != aCurrentFile)
    {
        prevFile = aCurrentFile;
        selectStyle(aCurrentFile);
        ui->mTextEdit->setTextChangedState(false);
    }
}

void UIDecorator::showUITemplates( const QStringList& aFiles, const QString& aCurrentFile )
{
    static QString prevFile;
    showFiles(ui->mComboBoxUITemplates,aFiles,aCurrentFile);
    if(prevFile != aCurrentFile)
    {
        prevFile = aCurrentFile;
        selectUITemplateFile(aCurrentFile);
    }
}

void UIDecorator::showFiles(QComboBox* aComboBox, const QStringList& aFiles, const QString& aCurrentFile)
{
    aComboBox->blockSignals(true);
    aComboBox->clear();
    aComboBox->addItems(aFiles);
    aComboBox->setCurrentText(aCurrentFile);
    aComboBox->blockSignals(false);
}

void UIDecorator::selectUITemplateFile(const QString &aFilename)
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

void UIDecorator::openUITemplateFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open UI template"), "", tr("UI Files (*.ui)"));

    if(!fileName.isEmpty())
        pROUTINER->addUITemplate(fileName);
}

void UIDecorator::newStyle()
{
    if( !checkStyle() )
        return;

    QString fileName = QFileDialog::getSaveFileName(this, tr("New CSS Style"), "", tr("CSS Files (*.css)"));

    if (!fileName.isEmpty())
        pROUTINER->newStyle(fileName, ui->mTextEdit->toPlainText());
}

void UIDecorator::openStyle()
{
    if( !checkStyle() )
        return;

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open CSS Style"), "", tr("CSS Files (*.css)"));

    for(auto fileName : files)
        pROUTINER->addStyle(fileName);
}

void UIDecorator::saveStyle()
{
    pROUTINER->saveStyle(ui->mTextEdit->toPlainText());
    ui->mTextEdit->setTextChangedState(false);
}

void UIDecorator::deleteStyle()
{
    auto res = QMessageBox::question(this, "UIDecorator", tr("Delete style: %1").arg(pROUTINER->getCurrentStyle()), QMessageBox::Yes|QMessageBox::No);
    if(res == QMessageBox::Yes)
        pROUTINER->deleteStyle();
}

void UIDecorator::applyStyle()
{
    if(mpCurrentWidget!=nullptr)
        mpCurrentWidget->setStyleSheet( ui->mTextEdit->toPlainText() );
}

bool UIDecorator::checkStyle()
{
    if(ui->mTextEdit->isTextChanged())
    {
        auto res = QMessageBox::question(this, "UIDecorator", tr("The style %1 has been changed.Do you want to save changes?").arg(pROUTINER->getCurrentStyle()), QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);

        if(res == QMessageBox::Yes)
        {
            saveStyle();
            return true;
        }

        if(res == QMessageBox::No)
            return true;

        return false;
    }

    return true;
}

void UIDecorator::selectStyle(const QString& aFilename)
{
    QFile file(aFilename);
    file.open(QFile::ReadOnly);
    if(file.isOpen())
    {
        QTextStream in(&file);
        ui->mTextEdit->setText(in.readAll());
        applyStyle();
        file.close();
    }
}

void UIDecorator::insertGradient(const QString &aProperty)
{
    QString gradient =  mpGradientDialog->getGradient();
    if(!gradient.isEmpty())
        ui->mTextEdit->insertLine( aProperty + " : " + gradient + ";");
}

void UIDecorator::insertResource(const QString &aProperty)
{
    QString resource = QFileDialog::getOpenFileName(this, tr("Open Image Style"), "", tr("Image Files (*.jpg *.png *.bmp *.svg)"));
    if(!resource.isEmpty())
        ui->mTextEdit->insertLine( aProperty + " :url(" + resource + ");");
}

void UIDecorator::insertColor(const QString& aProperty)
{
    QColor color = QColorDialog::getColor();
    ui->mTextEdit->insertLine( aProperty + " : " + color.name(QColor::NameFormat::HexArgb) + ";");
}

void UIDecorator::insertFont()
{
    bool ok;
    QFont font =QFontDialog::getFont(&ok);
    if(ok)
    {
        QString fontStyle = font.style() ? " italic" : "";
        QString fontWeight = font.bold() ? " bold" : "";
        ui->mTextEdit->insertLine(QString("font :%1%2 %3pt \"%4\";").arg(fontWeight).arg(fontStyle).arg(font.pointSize()).arg(font.family()));
    }
}
