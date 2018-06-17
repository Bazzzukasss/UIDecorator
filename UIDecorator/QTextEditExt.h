#ifndef QTEXTEDITEXT_H
#define QTEXTEDITEXT_H

#include <QTextEdit>
#include <QSyntaxHighlighterExt.h>

class QCompleter;
class QStandardItemModel;

struct Dictionary{
    QString mIcon;
    QStringList mWords;
};

class QTextEditExt : public QTextEdit
{
    Q_OBJECT

public:
    QTextEditExt(QWidget *parent = 0);
    ~QTextEditExt();

    void addDictionary(const QString& aFilename,const QString& aIconFilename = "");
    void clearDictionary();

    void addHighlightRule(const QString& aFilename, const QTextCharFormat& aFormat);
    void clearHighlightRules();

    bool isTextChanged() const;
    void setTextChangedState(bool aIsChanged);

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;

private slots:
    void insertCompletion(const QString &completion);

private:
    QCompleter *mCompleter;
    QSyntaxHighlighterExt* mHighlighter;

    QVector<Dictionary> mDictionaries;    

    QStandardItemModel* mModel{nullptr};
    bool mIsTextChanged{false};

    void initialize();
    void resetModel();
    void resetCompleter();
    bool isInDictionary(const QString& aWord);
    QString textUnderCursor() const;
};


#endif // QTEXTEDITEXT_H
