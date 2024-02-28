#ifndef A48213B2_FC46_48DB_8994_0AC8E86EB7C5
#define A48213B2_FC46_48DB_8994_0AC8E86EB7C5
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
        void newMessage(ChatKey chat_key, QString content) { qDebug("NotImplemented yet"); };
        void getParticipantKeys(ChatKey chatKey) { qDebug("NotImplemented yet"); };
        void leaveChat(ChatKey chatKey)
        {
                auto chat = std::find_if(chats.begin(), chats.end(), [&chatKey](std::shared_ptr<ServerChat> &chat)
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
        void joinChat(std::shared_ptr<ServerChat> chat)
        {
                chat->addParticipant(std::make_shared<ServerParticipant>(*this));
                chats.emplace_back(std::move(chat));
        }
        std::shared_ptr<ServerChat> getChat(ChatKey chatKey)
        {
                auto chat = std::find_if(chats.begin(), chats.end(), [&chatKey](std::shared_ptr<ServerChat> &chat)
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
};

#endif /* A48213B2_FC46_48DB_8994_0AC8E86EB7C5 */
