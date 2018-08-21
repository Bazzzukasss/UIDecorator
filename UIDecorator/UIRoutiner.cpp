#include "UIRoutiner.h"
#include <QSettings>
#include <QFile>
#include <QTextStream>

UIRoutiner *UIRoutiner::getInstace()
{
    static UIRoutiner rutiner;
    return &rutiner;
}

UIRoutiner::~UIRoutiner()
{
    saveSettings();
}

UIRoutiner::UIRoutiner(QObject *parent)
    : QObject(parent)
{
}

void UIRoutiner::initialize()
{
    mpSettings = new QSettings("config.ini",QSettings::IniFormat,this);
    loadSettings();
}

QString UIRoutiner::getCurrentStyle() const
{
    return mCurrentStyle;
}

QString UIRoutiner::getCurrentUITemplate() const
{
    return mCurrentUItemplate;
}

QString UIRoutiner::getCurrentResource() const
{
    return mCurrentResource;
}

void UIRoutiner::selectStyle(const QString &aFilename)
{
    if(!aFilename.isEmpty())
        mCurrentStyle = aFilename;
    emit signalStylesListChanged(mStyles,mCurrentStyle);
}

void UIRoutiner::addStyle(const QString &aFilename)
{
    if(!mStyles.contains(aFilename))
        mStyles.append(aFilename);
    mCurrentStyle = aFilename;
    emit signalStylesListChanged(mStyles,mCurrentStyle);
}

void UIRoutiner::newStyle(const QString &aFilename,const QString& aContent)
{
    addStyle(aFilename);
    saveStyle(aContent);
}

void UIRoutiner::saveStyle(const QString& aContent)
{
    QFile file(mCurrentStyle);

    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << aContent;
        file.close();
    }
}

void UIRoutiner::deleteStyle()
{
    QFile file(mCurrentStyle);
    file.remove();
    mStyles.removeOne(mCurrentStyle);
    if( mStyles.size() > 0 )
        mCurrentStyle = mStyles[0];
    else
        mCurrentStyle = "";
    emit signalStylesListChanged(mStyles,mCurrentStyle);
}

void UIRoutiner::selectUITemplate(const QString &aFilename)
{
    if(!aFilename.isEmpty())
        mCurrentUItemplate = aFilename;
    emit signalUITemplatesListChanged(mUITemplates,mCurrentUItemplate);
}

void UIRoutiner::addUITemplate(const QString &aFilename)
{
    if(!mUITemplates.contains(aFilename))
        mUITemplates.append(aFilename);
    emit signalUITemplatesListChanged(mUITemplates,mCurrentUItemplate);
}

void UIRoutiner::addRecource(const QString &aFilename)
{
    if(!mResources.contains(aFilename))
        mResources.append(aFilename);
    emit signalResourcesListChanged(mResources,mCurrentResource);
}

void UIRoutiner::selectResource(const QString &aFilename)
{
    if(!aFilename.isEmpty())
        mCurrentResource = aFilename;
    emit signalResourcesListChanged(mResources,mCurrentResource);
}

void UIRoutiner::saveSettings()
{
    //UITemplates
    int  i(0);
    mpSettings->beginGroup("UITemplates");
    mpSettings->beginWriteArray("files");
    for(auto& name : mUITemplates){
        mpSettings->setArrayIndex(i++);
        mpSettings->setValue("files",name);
    }
    mpSettings->endArray();
    mpSettings->endGroup();

    //Styles
    i=0;
    mpSettings->beginGroup("Styles");
    mpSettings->beginWriteArray("files");
    for(auto& name : mStyles){
        mpSettings->setArrayIndex(i++);
        mpSettings->setValue("files",name);
    }
    mpSettings->endArray();
    mpSettings->endGroup();

    //Resources
    mpSettings->beginGroup("Resources");
    mpSettings->beginWriteArray("files");
    i = 0;
    for(auto& name : mResources){
        mpSettings->setArrayIndex(i++);
        mpSettings->setValue("files",name);
    }
    mpSettings->endArray();
    mpSettings->endGroup();
}

void UIRoutiner::loadSettings()
{
    //UITemplates
    mUITemplates.clear();
    mpSettings->beginGroup("UITemplates");
    int size = mpSettings->beginReadArray("files");
    for(int i = 0; i < size; i++){
        mpSettings->setArrayIndex(i);
        mUITemplates.append( mpSettings->value("files").toString());
    }
    mpSettings->endArray();
    mpSettings->endGroup();

    if( mUITemplates.size() > 0 )
        mCurrentUItemplate = mUITemplates[0];
    emit signalUITemplatesListChanged(mUITemplates,mCurrentUItemplate);

    //Styles
    mStyles.clear();
    mpSettings->beginGroup("Styles");
    size = mpSettings->beginReadArray("files");
    for(int i = 0; i < size; i++){
        mpSettings->setArrayIndex(i);
        mStyles.append( mpSettings->value("files").toString());
    }
    mpSettings->endArray();
    mpSettings->endGroup();

    if( mStyles.size() > 0 )
        mCurrentStyle = mStyles[0];
    emit signalStylesListChanged(mStyles,mCurrentStyle);

    //Resources
    mResources.clear();
    mpSettings->beginGroup("Resources");
    size = mpSettings->beginReadArray("files");
    for(int i = 0; i < size; i++){
        mpSettings->setArrayIndex(i);
        mResources.append( mpSettings->value("files").toString());
    }
    mpSettings->endArray();
    mpSettings->endGroup();
    emit signalResourcesListChanged(mResources,mCurrentResource);
}
