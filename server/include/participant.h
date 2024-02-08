#include <QDateTime>
#include <QString>

#include "../../common/include/participant.h"
#include "chat.h"
#include "uniqueKey.h"
#include <algorithm>
#include <memory>


class ServerParticipant : public Participant
{
        std::vector<std::shared_ptr<ServerChat>> chats;

public:
        ServerParticipant() : Participant(UniqueKeyGenerator<ParticipantKey>::get_instance()->requestKey())
        {
                registeredSince = QDateTime::currentDateTime();
                name = "Unknown";
        };
        ChatKey newChat()
        {
                auto new_chat = std::make_shared<ServerChat>();
                auto new_chat_key = new_chat->getKey();
                chats.emplace_back(std::move(new_chat));
                return new_chat_key;
        }
        std::vector<ChatKey> getChatKeys()
        {
                std::vector<ChatKey> chatKeys;
                chatKeys.resize(chats.size());
                std::transform(chats.begin(), chats.end(), chatKeys.begin(), [](std::shared_ptr<ServerChat> &chat)
                               { return chat->getKey(); });
                return chatKeys;
        }
        /// @brief should be called from the Client thread when the clients sent a new message
        void newMessage(ChatKey &chat_key) { qDebug("NotImplemented yet"); };
        void getParticipantKeys(ChatKey &chat_key) { qDebug("NotImplemented yet"); };
        void leaveChat(ChatKey &chat_key) { qDebug("NotImplemented yet"); }
};
