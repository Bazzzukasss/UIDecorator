/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
    treeitem.cpp

    A container for items of data supplied by the simple tree model.
*/

#include "treeitem.h"

#include <QStringList>


TreeItem::TreeItem(const QVector<TreeItemData> &data, TreeItem *parent)
{
    mParent = parent;
    mData = data;
}

TreeItem::~TreeItem()
{
    qDeleteAll(mItems);
}

TreeItem *TreeItem::getItem(int getNumber)
{
    return mItems.value(getNumber);
}

const QList<TreeItem *>& TreeItem::getItems() const
{
    return mItems;
}

void TreeItem::addItem(TreeItem *item)
{
    mItems.append(item);
}

int TreeItem::itemsCount() const
{
    return mItems.count();
}

int TreeItem::getNumber() const
{
    if (mParent)
        return mParent->mItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::columnCount() const
{
    return mData.count();
}

bool TreeItem::insertItem(int position, int count, int columns)
{
    if (position < 0 || position > mItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<TreeItemData> data(columns);
        TreeItem *item = new TreeItem(data, this);
        mItems.insert(position, item);
    }

    return true;
}

bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > mData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        mData.insert(position, TreeItemData());

    foreach (TreeItem *child, mItems)
        child->insertColumns(position, columns);

    return true;
}

TreeItem *TreeItem::getParent()
{
    return mParent;
}

void TreeItem::setParent(TreeItem *parent)
{
    mParent = parent;
}

bool TreeItem::removeItem(int position, int count)
{
    if (position < 0 || position + count > mItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete mItems.takeAt(position);

    return true;
}

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > mData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        mData.remove(position);

    foreach (TreeItem *child, mItems)
        child->removeColumns(position, columns);

    return true;
}

bool TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= mData.size())
        return false;

    mData[column].second = value;
    return true;
}

bool TreeItem::setData(int column, const TreeItemData &value)
{
    if (column < 0 || column >= mData.size())
        return false;

    mData[column] = value;
    return true;
}

void TreeItem::setData(const QVector<TreeItemData> &data)
{
    mData = data;
}

QVariant TreeItem::getData(int column) const
{
    return mData.value(column).second;
}
const QVector<TreeItemData> &TreeItem::getData() const
{
    return mData;
}
