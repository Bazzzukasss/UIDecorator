#include "QTextEditExt.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QDebug>
#include <QApplication>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QScrollBar>
#include <set>

QTextEditExt::QTextEditExt(QWidget *parent)
    : QTextEdit(parent)
{
    initialize();
}

QTextEditExt::~QTextEditExt()
{
    delete mModel;
}

void QTextEditExt::initialize()
{
    mCompleter = new QCompleter(this);
    mCompleter->setWidget(this);
    mCompleter->setCompletionMode(QCompleter::PopupCompletion);
    mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    mCompleter->setWrapAround(false);
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

bool QTextEditExt::isInDictionary(const QString &aWord)
{
    for (const auto& dictionary : mDictionaries)
        if(dictionary.mWords.contains(aWord))
            return true;
    return false;
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
    cr.setWidth(mCompleter->popup()->sizeHintForColumn(0) + mCompleter->popup()->verticalScrollBar()->sizeHint().width());
    mCompleter->complete(cr); // popup it up!
}

void QTextEditExt::resetCompleter()
{
    mCompleter->setModel( mModel );
}
void QTextEditExt::resetModel()
{
    std::set< std::pair<std::string,std::string> > wordsSet;

    if(mModel != nullptr)
        delete mModel;

    for(auto& dictionary : mDictionaries)
        for(auto& word : dictionary.mWords)
            wordsSet.insert(std::pair<std::string,std::string>(word.toStdString(),dictionary.mIcon.toStdString()));

    mModel = new QStandardItemModel(0,1);

    for(auto& word : wordsSet)
    {
        QList<QStandardItem*> newRow;
        for (int i=0;i<mModel->columnCount();i++)
        {
            QIcon icon(QString().fromStdString(word.second));
            QString string = QString().fromStdString(word.first);

            QStandardItem* itm = new QStandardItem( icon, string);
            newRow.append(itm);
        }
        mModel->appendRow(newRow);
    }

}

void QTextEditExt::clearDictionary()
{
    mDictionaries.clear();
}

void QTextEditExt::addHighlightRule(const QString &aFilename, const QTextCharFormat &aFormat)
{
    mHighlighter->addRule(aFilename,aFormat);
}

void QTextEditExt::clearHighlightRules()
{
    mHighlighter->clearRules();
}

void QTextEditExt::addDictionary(const QString &aFilename, const QString &aIconFilename)
{
    QFile file(aFilename);
    if (file.open(QFile::ReadOnly))
    {
        QStringList wordsList;
        std::set<std::string> wordsSet;

        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            if (!line.isEmpty())
            {
                QStringList wordsInLine = QString(line.trimmed()).split(":");
                wordsSet.insert(wordsInLine.at(0).toStdString());
            }
        }
        for(auto& str : wordsSet)
            wordsList << QString().fromStdString(str);

        mDictionaries.push_back( {aIconFilename,wordsList} );
        resetModel();
        resetCompleter();
    }
}
