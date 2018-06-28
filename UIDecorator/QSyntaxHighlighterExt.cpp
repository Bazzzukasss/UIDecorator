#include "QSyntaxHighlighterExt.h"
#include <QDebug>

QSyntaxHighlighterExt::QSyntaxHighlighterExt(QTextDocument *parent)
    :QSyntaxHighlighter(parent)
{

}

void QSyntaxHighlighterExt::addRule(const QRegularExpression &aExpression, const QTextCharFormat &aFormat)
{
    qDebug()<<aExpression;
    mRules.push_back({aExpression,aFormat});
}

void QSyntaxHighlighterExt::addRule(const QStringList &aPatterns, const QTextCharFormat &aFormat)
{
    for(auto& pattern : aPatterns  )
        addRule(QRegularExpression(pattern),aFormat);
}

void QSyntaxHighlighterExt::addRule(const QString &aFilename, const QTextCharFormat &aFormat)
{
    QFile file(aFilename);
    if (file.open(QFile::ReadOnly))
    {
        QStringList wordsList;

        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            if (!line.isEmpty())
            {
                QStringList wordsInLine = QString(line.trimmed()).split("#");
                wordsList << wordsInLine.at(0);
            }
        }
        addRule(wordsList,aFormat);
    }
}

void QSyntaxHighlighterExt::clearRules()
{
    mRules.clear();
}

void QSyntaxHighlighterExt::highlightBlock(const QString &aText)
{
    foreach (const HighlightingRule &rule, mRules) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(aText);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
        setCurrentBlockState(0);
    /*
        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = text.indexOf(commentStartExpression);

        while (startIndex >= 0) {
            QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex
                                + match.capturedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
        */
}
