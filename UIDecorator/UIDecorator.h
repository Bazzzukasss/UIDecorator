#ifndef UIDECORATOR_H
#define UIDECORATOR_H

#include <QFrame>
#include <QStringList>
#include <QVBoxLayout>

class QTextCharFormat;
class QSettings;
class GradientDialog;
class QComboBox;

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
    QWidget* mpCurrentWidget{nullptr};
    QVBoxLayout* mpLayout{nullptr};
    GradientDialog* mpGradientDialog;

    void selectUITemplateFile(const QString&);
    void openUITemplateFile();

    void newStyle();
    void openStyle();
    void saveStyle();
    void deleteStyle();
    void applyStyle();
    bool checkStyle();
    void selectStyle(const QString&);

    void insertGradient(const QString& aProperty = "image");
    void insertPreset(const QString& aFilename = "");
    void insertResource(const QString& aProperty = "image");
    void insertColor(const QString &aProperty = "color");
    void insertFont();

    void initialize();
    void initializeDictionaries();
    void initializeDictionaries(const QString&, const QString, const QTextCharFormat&);


    void showStyles(const QStringList&,const QString&);
    void showUITemplates(const QStringList&, const QString &);
    void showFiles(QComboBox*, const QStringList&, const QString &);
};

#endif // UIDECORATOR_H
