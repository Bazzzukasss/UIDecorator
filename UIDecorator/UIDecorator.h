#ifndef UIDECORATOR_H
#define UIDECORATOR_H

#include <QFrame>
#include <QStringList>
#include <QVBoxLayout>

class ResourceDialog;
class QTextCharFormat;
class QSettings;
class GradientDialog;

namespace Ui {
class UIDecorator;
}

#define STYLE_FOLDER "Styles"


class UIDecorator : public QFrame
{
    Q_OBJECT

public:
    explicit UIDecorator(QWidget *parent = 0);
    ~UIDecorator();

private:
    Ui::UIDecorator *ui;
    QStringList mStyles;
    QStringList mUITemplates;
    QString mCurrentUItemplate;
    QString mCurrentStyle;
    QWidget* mpCurrentWidget{nullptr};
    QVBoxLayout* mpLayout{nullptr};
    bool isStyleChanged{false};
    QString mCurrentStyleName;
    QSettings* mSettings;
    ResourceDialog* mResourceDialog;
    GradientDialog* mGradientDialog;

    void refreshStylesList(const QString& aFoldername = STYLE_FOLDER);
    void refreshUITemplatesList();

    void selectUITemplate(const QString& aFilename);
    void addUITemplate();

    void loadStyle(const QString& aFilename);
    void newStyle();
    void saveStyle(const QString& aFilename);
    void deleteStyle(const QString& aFilename);
    void applyStyle();
    void selectStyle(const QString& aFilename);

    void addGradient(const QString& aProperty = "image");
    void addResource(const QString& aProperty = "image");
    void addColor(const QString &aProperty = "color");
    void addFont();

    void saveSettings();
    void initialize();
    void initializeSettings();
    void initializeDictionaries();
    void initializeDictionaries(const QString& aFilename, const QString aIcon, const QTextCharFormat& aFormat);
};

#endif // UIDECORATOR_H
