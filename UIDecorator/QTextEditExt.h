#ifndef QTEXTEDITEXT_H
#define QTEXTEDITEXT_H

#include <QTextEdit>
class QCompleterExt;
class QSyntaxHighlighterExt;

class QTextEditExt : public QTextEdit
{
    Q_OBJECT

public:
    QTextEditExt(QWidget *parent = 0);
    ~QTextEditExt();

    void addCompletionDictionary(const QString& aFilename,const QString& aIconFilename = "");
    void clearCompletionDictionary();

    void addHighlightionRule(const QString& aFilename, const QTextCharFormat& aFormat);
    void clearHighlightionRules();

    bool isTextChanged() const;
    void setTextChangedState(bool aIsChanged);

    void insertLine(const QString& aLine);

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;

private slots:
    void insertCompletion(const QString &completion);

private:
    QCompleterExt *mCompleter;
    QSyntaxHighlighterExt* mHighlighter;
    bool mIsTextChanged{false};
    QString mCurrentWord;
    int mWordStartPosition;
    int mWordEndPosition;

    void initialize();
    QString textUnderCursor();
};


#endif // QTEXTEDITEXT_H
