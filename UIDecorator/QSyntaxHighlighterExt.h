#ifndef QSYNTAXHIGHLIGHTEREXT_H
#define QSYNTAXHIGHLIGHTEREXT_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

struct HighlightingRule
{
    QRegularExpression pattern;
    QTextCharFormat format;
};

class QSyntaxHighlighterExt : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    QSyntaxHighlighterExt(QTextDocument *parent = 0);

    void addRule(const QRegularExpression& aExpression, const QTextCharFormat& aFormat);
    void addRule(const QStringList& aPatterns, const QTextCharFormat& aFormat);
    void addRule(const QString& aFilename, const QTextCharFormat& aFormat);
    void clearRules();

protected:
    void highlightBlock(const QString &aText) override;

private:
    QVector<HighlightingRule> mRules;
};

#endif // QSYNTAXHIGHLIGHTEREXT_H
