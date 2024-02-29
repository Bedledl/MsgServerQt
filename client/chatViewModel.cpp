#include "chatViewModel.h"
#include <QModelIndex>

#include "messageModelItem.h"

#include "moc_chatViewModel.cpp"

int ChatViewModel::rowCount(const QModelIndex &parent) const
{
    return chat.getMessagesCount();
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
        try {
            auto message = chat.getMessageAt(index.row());
        return QVariant::fromValue(new MessageModelItem(&message));
        } catch (std::out_of_range &e) {
            qDebug() << "ChatViewModel::data() requested data which is out of range";
            return QVariant();
        }

    }
    default:
        return "default role string";
        break;
    }
}
