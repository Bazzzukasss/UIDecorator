#ifndef XMLDATA_H
#define XMLDATA_H

#include <QString>
#include <QXmlStreamAttributes>
#include <QVariant>
#include <QVector>

class XMLData
{
    enum TagID{TAG_NAME,TAG_VALUE,TAG_ATTRIBUTES};
public:
    XMLData()
    {
    }

    XMLData(const QVector<TreeItemData>& data)
        :mData(data)
    {
        mName = data[TAG_NAME].second.toString();
        mValue = data[TAG_VALUE].second.toString();
        int i=0;
        for(auto a : data)
        {
            if(i>1)
                mAttributes.push_back(QXmlStreamAttribute(a.first,a.second.toString()));
            i++;
        }
    }

    QVector< QPair<QString,QVariant>> getData()
    {
        mData.clear();
        mData.push_back(TreeItemData("tag",mName));
        mData.push_back(TreeItemData("value",mValue));
        for(auto a : mAttributes)
            mData.push_back(TreeItemData( a.name().toString(), QVariant().fromValue(a.value().toString())));
        return mData;
    }

    QString getName()const{return mName;}
    QString getValue()const{return mValue;}
    QXmlStreamAttributes getAttributes()const{return mAttributes;}

    void setName(const QString& name){mName = name;}
    void setValue(const QString& value){mValue = value;}
    void setAttributes(const QXmlStreamAttributes& attributes){mAttributes = attributes;}

private:
    QString mName;
    QString mValue;
    QXmlStreamAttributes mAttributes;
    QVector< QPair<QString,QVariant>> mData;
};

#endif // XMLDATA_H
