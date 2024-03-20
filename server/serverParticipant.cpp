#include "serverParticipant.h"
#include "uniqueKeysTypedef.h"
#include <QDateTime>
#include <QString>
#include <memory>

ServerParticipant::ServerParticipant() : Participant(UniqueKeyGenerator<ParticipantKey>::get_instance()->requestKey())
{
    registeredSince = QDateTime::currentDateTime();
    name = "Unknown";
};

ChatKey ServerParticipant::newChat()
{
    auto new_chat = std::make_shared<ServerChat>();
    auto new_chat_key = new_chat->getKey();
    chats.emplace_back(std::move(new_chat));
    return new_chat_key;
}
std::vector<ChatKey> ServerParticipant::getChatKeys() const
{
    std::vector<ChatKey> chatKeys;
    chatKeys.resize(chats.size());
    std::transform(chats.begin(), chats.end(), chatKeys.begin(), [](const std::shared_ptr<ServerChat> &chat)
                   { return chat->getKey(); });
    return chatKeys;
}
/// @brief should be called from the Client thread when the clients sent a new message
void ServerParticipant::newMessage(ChatKey chat_key, std::string content, QDateTime datetime)
{
    auto chat = std::find_if(chats.begin(), chats.end(), [&chat_key](const std::shared_ptr<ServerChat> &chat)
                             { return chat->getKey() == chat_key; });
    if (chat != chats.end())
    {
        (*chat)->addMessage(QString::fromStdString(content), std::make_shared<ServerParticipant>(*this), datetime);
    }
    else
    {
        qDebug() << "ServerParticipant::newMessage: Chat not found";
        throw ChatNotFound();
    }
};
std::vector<ParticipantKey> ServerParticipant::getParticipantKeys(ChatKey chatKey) const
{
    auto chat = std::find_if(chats.begin(), chats.end(), [&chatKey](const std::shared_ptr<ServerChat> &chat)
                             { return chat->getKey() == chatKey; });
    if (chat != chats.end())
    {
        auto keys = (*chat)->getParticipantKeys();
        return keys;
    }
    else
    {
        qDebug() << "ServerParticipant::getParticipantKeys: Chat not found";
        throw ChatNotFound();
    }
};
void ServerParticipant::leaveChat(ChatKey chatKey)
{
    auto chat = std::find_if(chats.begin(), chats.end(), [&chatKey](const std::shared_ptr<ServerChat> &chat)
                             { return chat->getKey() == chatKey; });
    if (chat != chats.end())
    {
        (*chat)->removeParticipant(getKey());
        std::erase(chats, *chat);
    }
    else
    {
        qDebug() << "ServerParticipant::LeaveChat: Chat not found";
        throw ChatNotFound();
    }
}
void ServerParticipant::joinChat(std::shared_ptr<ServerChat> chat)
{
    chat->addParticipant(std::make_shared<ServerParticipant>(*this));
    chats.emplace_back(std::move(chat));
}
std::shared_ptr<ServerChat> ServerParticipant::getChat(ChatKey chatKey) const
{
    auto chat = std::find_if(chats.begin(), chats.end(), [&chatKey](const std::shared_ptr<ServerChat> &chat)
                             { return chat->getKey() == chatKey; });
    if (chat != chats.end())
    {
        return *chat;
    }
    else
    {
        qDebug() << "ServerParticipant::getChat: Chat not found";
        throw ChatNotFound();
    }
}
