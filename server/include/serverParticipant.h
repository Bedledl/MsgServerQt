#ifndef A48213B2_FC46_48DB_8994_0AC8E86EB7C5
#define A48213B2_FC46_48DB_8994_0AC8E86EB7C5
#include <QDateTime>
#include <QString>

#include "chat.h"
#include "participant.h"
#include "uniqueKey.h"
#include <algorithm>
#include <memory>

class ServerParticipant : public Participant
{
    std::vector<std::shared_ptr<ServerChat>> chats;

public:
    /**
     * Constructor of the ServerParticipant class.
     * Initializes the registeredSince date with the current date and time and the name with "Unknown".
     */
    ServerParticipant();
    /**
     * Creates a new Chat and returns its key. The new Chat is added to the chats vector.
     * @return The key of the new Chat.
     */
    virtual ChatKey newChat();
    /**
     * Returns a vector of all Chat keys this participant is in.
     * @return A vector of Chat keys.
     */
    virtual std::vector<ChatKey> getChatKeys() const;
    /**
     * This method is called when the respective client sends a new message on a chat.
     * Adds the message to the Chat specified by chat_key.
     * @param chat_key The key of the Chat the message should be added to.
     * @param content The content of the message.
     * @param datetime The date and time the message was sent.
     * @throw ChatNotFound If the Chat specified by chat_key couldn't be found.
     */
    virtual void newMessage(ChatKey chat_key, std::string content, QDateTime datetime);
    /**
     * Returns a vector of all Participant keys that are in the Chat specified by chatKey.
     * @param chatKey The key of the Chat to get the Participant keys from.
     * @return A vector of Participant keys.
     */
    virtual std::vector<ParticipantKey> getParticipantKeys(ChatKey chatKey) const;
    /**
     * Removes the Participant from the Chat specified by chatKey.
     * @param chatKey The key of the Chat the Participant should leave.
     */
    virtual void leaveChat(ChatKey chatKey);
    /**
     * Adds the Participant to the Chat specified by chat.
     * @param chat A shared_ptr to the Chat the Participant should join.
     */
    virtual void joinChat(std::shared_ptr<ServerChat> chat);
    /**
     * Returns a shared_ptr to the Chat specified by chatKey.
     * @param chatKey The key of the Chat the Participant wants to get.
     * @return A shared_ptr to the Chat specified by chatKey or a nullptr if the Chat couldn't be found.
     */
    virtual std::shared_ptr<ServerChat> getChat(ChatKey chatKey) const;
};

#endif /* A48213B2_FC46_48DB_8994_0AC8E86EB7C5 */
