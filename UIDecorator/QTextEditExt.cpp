#include "QTextEditExt.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QDebug>
#include <QApplication>
#include <QScrollBar>
#include <set>
#include "QCompleterExt.h"
#include "QSyntaxHighlighterExt.h"

QTextEditExt::QTextEditExt(QWidget *parent)
    : QTextEdit(parent)
{
    initialize();
}

QTextEditExt::~QTextEditExt()
{    
}

void QTextEditExt::initialize()
{
    mCompleter = new QCompleterExt(this);
    mCompleter->setWidget(this);
    mCompleter->setCompletionMode(QCompleter::PopupCompletion);
    mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    mCompleter->setWrapAround(false);
    mCompleter->setCompletionColumn(0);

    connect(mCompleter,    SIGNAL(activated(QString)), this,   SLOT(insertCompletion(QString)));
    connect(this,&QTextEditExt::textChanged,[&](){ mIsTextChanged = true; });

    mHighlighter = new QSyntaxHighlighterExt(document());
}

void QTextEditExt::insertCompletion(const QString& completion)
{
    if (mCompleter->widget() != this)
        return;
    QTextCursor cursor = textCursor();

    cursor.setPosition(mWordStartPosition, QTextCursor::MoveAnchor);
    cursor.setPosition(mWordEndPosition, QTextCursor::KeepAnchor);

    cursor.removeSelectedText();
    cursor.insertText(completion);

    setTextCursor(cursor);
}

QString QTextEditExt::textUnderCursor()
{
    QTextCursor cursor = textCursor();
    QString text = toPlainText();

    cursor.select(QTextCursor::WordUnderCursor);
    QString mCurrentWord = cursor.selectedText();

    int startPosition = cursor.anchor();
    mWordStartPosition = startPosition;

    //remove none letters from end of the word under cursor
    int length = mCurrentWord.length();
    if( length > 0)
    {
       auto ch = mCurrentWord.right(1).at(0);
       if(!ch.isLetterOrNumber() && ch != ":")
           mCurrentWord = mCurrentWord.left(length - 1);
    }

    //addition : or :: to word under cursor
    if(startPosition > 1)
    {
        auto ch = text[--startPosition];
        while((ch == ":") && (startPosition > 0))
        {
            //qDebug()<<ch;
            mCurrentWord =QString(":%1").arg(mCurrentWord);
            mWordStartPosition = startPosition;
            ch = text[--startPosition];
        }
    }

    //addition letters to empty word under cursor
    if(mCurrentWord.isEmpty())
    {
        auto ch = text[startPosition];
        while( ch.isLetter() && (startPosition > 0))
        {
            //qDebug()<<ch;
            mCurrentWord =QString("%1%2").arg(ch).arg(mCurrentWord);
            mWordStartPosition = startPosition;
            ch = text[--startPosition];
        }
    }

    mWordEndPosition = mWordStartPosition + mCurrentWord.length();
    //qDebug()<<mCurrentWord;
    return mCurrentWord;
}

bool QTextEditExt::isTextChanged() const
{
    return mIsTextChanged;
}

void QTextEditExt::setTextChangedState(bool aIsChanged)
{
    mIsTextChanged = aIsChanged;
}

void QTextEditExt::insertLine(const QString &aLine)
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor);
    setTextCursor(cursor);
    insertPlainText(aLine);
}

void QTextEditExt::focusInEvent(QFocusEvent *e)
{
    if (mCompleter)
        mCompleter->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void QTextEditExt::keyPressEvent(QKeyEvent *e)
{
    if (mCompleter && mCompleter->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!mCompleter || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!mCompleter || (ctrlOrShift && e->text().isEmpty()))
        return;


    //static QString eow("~!@#%^&*()_+{}:|\"<>?,./;'[]\\="); // end of word
    static QString eow("~!@#%^&*()_+|\"<>?,./;'[]\\="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    auto v1 = isShortcut;
    auto v2 = hasModifier;
    auto v3 = e->text().isEmpty();
    auto v4 = completionPrefix.length();
    auto v5 = eow.contains(e->text().right(1));
   if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1 || eow.contains(e->text().right(1))) )
    {
        mCompleter->popup()->hide();
        return;
    }

    if (completionPrefix != mCompleter->completionPrefix())
    {
        mCompleter->setCompletionPrefix(completionPrefix);
        mCompleter->popup()->setCurrentIndex(mCompleter->completionModel()->index(0, 0));
    }

    QRect cr = cursorRect();
    //cr.setWidth(mCompleter->popup()->sizeHintForColumn(0) + mCompleter->popup()->sizeHintForColumn(1) + mCompleter->popup()->verticalScrollBar()->sizeHint().width());
    cr.setWidth(width());
    mCompleter->complete(cr); // popup it up!
}

void QTextEditExt::addHighlightionRule(const QString &aFilename, const QTextCharFormat &aFormat)
{
    mHighlighter->addRule(aFilename,aFormat);
}

void QTextEditExt::clearHighlightionRules()
{
    mHighlighter->clearRules();
}

void QTextEditExt::addCompletionDictionary(const QString &aFilename, const QString &aIconFilename)
{
    mCompleter->addDictionary(aFilename,aIconFilename);
}

void QTextEditExt::clearCompletionDictionary()
{
    mCompleter->clearDictionaries();
}
