#include "chatViewModel.h"
#include <QModelIndex>

#include "messageModelItem.h"

#include "moc_chatViewModel.cpp"

int ChatViewModel::rowCount(const QModelIndex &parent) const
{
    return messages.size();
}

QVariant ChatViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DisplayRole:
    {
        auto it = messages.begin();
        std::advance(it, index.row());
        return QVariant::fromValue(new MessageModelItem(&(*it)));
    }
    default:
        return "default role string";
        break;
    }
}
