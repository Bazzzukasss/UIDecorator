#ifndef QTEXTEDITEXT_H
#define QTEXTEDITEXT_H

#include <QTextEdit>

class QCompleter;
class QStandardItemModel;

struct Decoration{
    Decoration(){
        mFont.setBold(true);
    }

    Decoration(const QColor& aColor, const QFont& aFont)
        : mColor(aColor)
        , mFont(aFont)
    {
        mFont.setBold(true);
    }
    QColor mColor{QColor(Qt::darkGray)};
    QFont mFont{QFont("MS Shell Dlg 2")};
};

struct Dictionary{
    Decoration mDecoration;
    QString mIcon;
    QStringList mWords;
};

class QTextEditExt : public QTextEdit
{
    Q_OBJECT

public:
    QTextEditExt(QWidget *parent = 0);
    ~QTextEditExt();

    void addDictionary(const QString& aFilename,const Decoration& aDecoration,const QString& aIconFilename = "");
    void clearDictionary();
    void formatText();
    bool isTextChanged() const;
    void setTextChangedState(bool aIsChanged);

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;

private slots:
    void insertCompletion(const QString &completion);

private:
    QString textUnderCursor() const;
    QCompleter *mCompleter;
    QVector<Dictionary> mDictionaries;
    QStandardItemModel* mModel{nullptr};
    bool mIsTextChanged{false};

    void initialize();
    void resetModel();
    void resetCompleter();

    QStringList getWordList(const QString& aLine);
    Decoration getWordDecoration(const QString& word);
    bool isInDictionary(const QString& word);

    QString formatLine(const QString& aLine);
    QString formatWord(const QString& aWord);
};


#endif // QTEXTEDITEXT_H
