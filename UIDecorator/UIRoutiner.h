#ifndef UIRUTINER_H
#define UIRUTINER_H

#include <QObject>

#define pROUTINER UIRoutiner::getInstace()

class QSettings;

class UIRoutiner : public QObject
{
    Q_OBJECT

public:
    static UIRoutiner* getInstace();
    ~UIRoutiner();

    void selectStyle(const QString& aFilename);
    void addStyle(const QString& aFilename);
    void newStyle(const QString& aFilename,const QString& aContent);
    void saveStyle(const QString& aContent);
    void deleteStyle();

    void selectUITemplate(const QString& aFilename);
    void addUITemplate(const QString& aFilename);

    void initialize();

    QString getCurrentStyle() const;
    QString getCurrentUITemplate() const;
    QString getCurrentResource() const;

signals:
    void signalStylesListChanged(const QStringList&,const QString&);
    void signalUITemplatesListChanged(const QStringList&,const QString&);

private:
    QStringList mStyles;
    QStringList mUITemplates;
    QString mCurrentUItemplate;
    QString mCurrentStyle;
    QString mCurrentResource;
    QSettings* mpSettings;

    void loadSettings();
    void saveSettings();

    explicit UIRoutiner(QObject *parent = nullptr);
    UIRoutiner(const UIRoutiner&);
    UIRoutiner& operator=(const UIRoutiner&);
};

#endif // UIRUTINER_H
