#include <QFile>
#include <QDebug>

#include "xmlprocessor.h"
#include "xmldata.h"

XMLProcessor* XMLProcessor::mInstance = nullptr;

XMLProcessor *XMLProcessor::getInstance()
{
    if(mInstance == nullptr)
        mInstance = new XMLProcessor();
    return mInstance;
}

XMLProcessor::~XMLProcessor()
{
    delete mInstance;
}

bool XMLProcessor::Load(const QString &fileName, TreeItem *item)
{
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QXmlStreamReader reader(&file);
        reader.isStartDocument();
        load(reader,item);

        if (reader.hasError())
        {
            //emit signalError(tr("Failed loading file:\n\"%1\"\nError:\n%2").arg(file.fileName()).arg(reader.errorString()));
            return false;
        }

        return true;
    }
    return false;
}

bool XMLProcessor::Save(const QString &fileName, const TreeItem *item)
{
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QXmlStreamWriter writer(&file);

        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        save(writer,item);
        writer.writeEndDocument();

        if (writer.hasError())
        {
            //emit error(tr("Failed to save configuration file:\n\"%1\"\n").arg(file.fileName()));
            return false;
        }
        return true;
    }
    return false;
}

bool XMLProcessor::load(QXmlStreamReader &reader, TreeItem *item)
{
    std::vector<XMLData> stack;
    XMLData data;
    TreeItem* currentItem=item;
    TreeItem* subItem;
    size_t stackSize = 0;

    while (!reader.atEnd())
    {
        reader.readNext();
        switch (reader.tokenType())
        {
            case QXmlStreamReader::StartElement:

                data.setAttributes(reader.attributes());
                data.setName(reader.name().toString());
                data.setValue(QString());

                subItem = new TreeItem(data.getData(),currentItem);
                currentItem->addItem( subItem );
                currentItem = subItem;

                if (stackSize < stack.size())
                    stack.resize(stackSize);

                stack.emplace_back(data);
                ++stackSize;

                break;
            case QXmlStreamReader::EndElement:
                if (stackSize == stack.size())
                    currentItem->setData(stack.back().getData());
                --stackSize;
                currentItem = currentItem->getParent();
                break;
            case QXmlStreamReader::Characters:
                stack.back().setValue(reader.text().toString());
                break;
            default:
                break;
        }
    }

    return true;
}

bool XMLProcessor::save(QXmlStreamWriter &writer, const TreeItem *item)
{
    for (TreeItem* subItem : item->getItems())
    {
        XMLData data(subItem->getData());
        writer.writeStartElement(data.getName());
        writer.writeAttributes(data.getAttributes());
        if(!data.getValue().isNull())
            writer.writeCharacters(data.getValue());
        save(writer,subItem);
        writer.writeEndElement();
    }

    return true;
}

