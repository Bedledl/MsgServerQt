#ifndef CHAT_H
#define CHAT_H

#include <QDate>
#include <QObject>
#include <QReadWriteLock>

#include <set>
#include <memory>

#include "message.h"
#include "participant.h"

typedef unsigned ChatKey;

class SenderNotChatParticipant : public std::exception
{
public:
    const char *what()
    {
        return "The message's sender is not part of this chat.\n";
    }
};

class ChatNotFound : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Chat not found.";
    }
};

class ChatAlreadyExists : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Chat already exists.";
    }
};

/**
 * @brief The Chat class represents a chat conversation.
 *
 * This class inherits from QObject and provides functionality for managing chat messages and participants.
 * A chat has a unique key and a set of messages. It also has a list of participants.
 * The unique key on server side should be created using the UniqueKeyGenerator<ServerChat> class.
 * On client side, the client is informed about the chat's key by the server.
 * The chat key identifies the chat in the network communication.
 */
class Chat : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Chat object with the given key and parent.
     *
     * @param key The key associated with the chat.
     * @param parent The parent QObject of the chat.
     */
    Chat(ChatKey key, QObject *parent = nullptr) : QObject(parent), key(key)
    {
        messages = std::set<Message>();
    };
    /**
     * Applies the given function to each message in the chat.
     *
     * @param func The function to apply to each message.
     */
    void foreach_do(std::function<void(const Message &)> func);
    /**
     * @brief Get the key associated with the chat.
     *
     * @return ChatKey The key associated with the chat.
     */
    ChatKey getKey() const { return key; };
    /**
     * @brief Adds a message to the chat.
     *
     * This function adds a message to the chat. The message is represented by a Message object.
     * Consider using move semantics to avoid copying the message.
     *
     * @param msg The message to be added.
     */
    void addMessage(QString content, std::shared_ptr<Participant> sender, QDateTime timestamp);
    /**
     * @brief Adds a participant to the chat.
     *
     * This function adds a participant to the chat. The participant is represented by a shared pointer to a Participant object.
     *
     * @param sender A shared pointer to the Participant object representing the participant to be added.
     */
    void addParticipant(std::shared_ptr<Participant> participant);
    /**
     * @brief Remove participant from the chat.
     *
     * @param participant: ParticipantKey of participant that should be removed from chat
     * @throw ParticipantNotFound: if participant was not found in participant list of chat
     */
    void removeParticipant(ParticipantKey participantKey);
    /**
     * @brief Returns the last message in the chat.
     *
     * @return const Message& The reference to the last message.
     * @throw std::out_of_range if the chat is empty.
     */
    const Message &getLastMessage() const;
    /**
     * @brief Retrieves the message at the specified index.
     *
     * @param index The index of the message to retrieve.
     * @return const Message& A reference to the message at the specified index.
     * @throw std::out_of_range if the index is invalid.
     */
    const Message &getMessageAt(size_t index) const;
    std::vector<ParticipantKey> getParticipantKeys();

public slots:
    /**
     * @brief Adds a message to the chat.
     *
     * This function adds a message to the chat. The message is represented by a Message object.
     * Consider using move semantics to avoid copying the message.
     *
     * @param msg The message to be added.
     */
    void addMessage(Message msg);
    /**
     * @brief Adds a message to the chat.
     *
     * This function adds a message to the chat.
     * The Message object is created inplace with the specified content, sender, and timestamp.
     *
     * @param content The content of the message.
     * @param sender The participant who sent the message.
     * @param timestamp The timestamp when the message was sent.
     */
    void addMessage(QString content, std::shared_ptr<Participant> sender, QDateTime timestamp);

signals:

    /**
     * @brief Notifies that the order of messages has been invalidated.
     *
     * This function is called when the order of messages in the chat has been invalidated,
     * such as when a new message is added in the middle of the chat.
     *
     */
    void messageOrderInvalidated();
    /**
     * @brief Notifies that a message has been appended to the end of the chat.
     *
     * This function is called to indicate that a new message has been added to the end of the chat.
     * It can be used to trigger any necessary actions or updates in the user interface.
     * If the message was added to the chat and it can not be assured that it was appended to the end of the chat,
     * the messageOrderInvalidtated signal should be emitted instead.
     */
    void messageAppended();

protected:
    const ChatKey key;
    std::set<Message> messages;
    std::vector<std::shared_ptr<Participant>> participants;
    mutable QReadWriteLock mutex;
};

#endif
