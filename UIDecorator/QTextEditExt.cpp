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

    mHighlighter = new QSyntaxHighlighterExt(document());
}

void QTextEditExt::insertCompletion(const QString& completion)
{
    if (mCompleter->widget() != this)
        return;
    QTextCursor tc = textCursor();
    tc.movePosition(QTextCursor::EndOfWord);
    tc.select(QTextCursor::WordUnderCursor);
    tc.removeSelectedText();
    tc.insertText(completion);
    setTextCursor(tc);
}

QString QTextEditExt::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

bool QTextEditExt::isTextChanged() const
{
    return mIsTextChanged;
}

void QTextEditExt::setTextChangedState(bool aIsChanged)
{
    mIsTextChanged = aIsChanged;
}

void QTextEditExt::focusInEvent(QFocusEvent *e)
{
    if (mCompleter)
        mCompleter->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void QTextEditExt::keyPressEvent(QKeyEvent *e)
{
    mIsTextChanged = true;
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


    static QString eow("~!@#%^&*()_+{}|:\"<>?,./;'[]\\="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

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
