#include <QDateTime>

#include <stdexcept>

#include "chat.h"
#include "chatPreviewModel.h"
#include <iostream>

#include "moc_chatPreviewModel.cpp"

QString ChatPreviewModel::chatName() const
{
    qDebug() << "ChatPreviewModel::chatName()";
    return "chatName";
}
QString ChatPreviewModel::lastMessage() const
{
    qDebug() << "ChatPreviewModel::lastMessage()";
    try
    {
        auto lastMsg = chat->getLastMessage();
        return lastMsg.getContent();
    }
    catch (const std::out_of_range &)
    {
        return "No messages yet.";
    }
}
QDateTime ChatPreviewModel::lastMessageTimestamp() const
{
    qDebug() << "ChatPreviewModel::lastMessageTimestamp()";
    return QDateTime::currentDateTime();
}

uint ChatPreviewModel::getChatKey() const
{
    qDebug() << "ChatPreviewListModel::chatKey()";
    return chat->getKey();
}

// and now implementations for the preview list

void ChatPreviewListModel::addChat(Chat *chat)
{
    emit beginInsertRows(QModelIndex(), chatsPreviews.length(), chatsPreviews.length());
    chatsPreviews.append(new ChatPreviewModel(chat));
    emit endInsertRows();
}
void ChatPreviewListModel::removeChat(Chat *chat)
{
    auto it = std::find_if(chatsPreviews.begin(), chatsPreviews.end(), [&chat](auto &cpm)
                           { return chat == cpm->chat; });
    if (it != chatsPreviews.end())
    {
        emit beginRemoveRows(QModelIndex(), std::distance(chatsPreviews.begin(), it), std::distance(chatsPreviews.begin(), it));
        chatsPreviews.removeOne(*it);
        emit endRemoveRows();
    }
}
ChatPreviewListModel::ChatPreviewListModel(QMap<ChatKey, Chat *> *chats, QObject *parent) : QAbstractListModel(parent)
{
    for (auto chat : *chats)
    {
        addChat(chat);
    }
};

int ChatPreviewListModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "ChatPreviewListModel::rowCount()" << parent.column() << ", " << parent.row();
    if (parent.isValid())
    {
        qDebug() << "ChatPreviewListModel::rowCount()" << chatsPreviews.count();
        return chatsPreviews.count();
    }
    // TODO why is the parent invalid most of the time???
    return chatsPreviews.count();
    return 0;
};

QVariant ChatPreviewListModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "ChatPreviewListModel::data()";
    if (!index.isValid())
    {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DisplayRole:
    {
        auto it = chatsPreviews.begin();
        std::advance(it, index.row());
        return QVariant::fromValue(*it);
    }
    default:
        return "default role string";
        break;
    }
};
