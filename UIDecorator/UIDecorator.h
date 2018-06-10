#ifndef UIDECORATOR_H
#define UIDECORATOR_H

#include <QFrame>
#include <QStringList>
#include <QVBoxLayout>

namespace Ui {
class UIDecorator;
}

#define STYLE_FOLDER "Styles"
#define UITEMPLATES_FOLDER "UITemplates"

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

    void refreshStylesList(const QString& aFoldername = STYLE_FOLDER);
    void refreshUITemplatesList(const QString& aFoldername = UITEMPLATES_FOLDER);

    void loadUITemplate(const QString& aFilename);
    void loadStyle(const QString& aFilename);
    void newStyle();
    void saveStyle(const QString& aFilename);
    void deleteStyle(const QString& aFilename);
    void applyStyle();
    void selectStyle(const QString& aFilename);

    void initialize();
    void initializeDictionaries();
};

#endif // UIDECORATOR_H
