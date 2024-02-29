#ifndef CHAT_H
#define CHAT_H

#include <QDate>
#include <QObject>
#include <QReadWriteLock>

#include <set>

#include "message.h"
#include "participant.h"

typedef unsigned ChatKey;

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

class Chat : public QObject
{
    Q_OBJECT

    std::set<Message> messages = {};

public:
    Chat(ChatKey key, QObject *parent = nullptr) : QObject(parent), key(key){};
    void foreach_do(std::function<void(const Message &)> func);
    ChatKey getKey() const { return key; };
    void addMessage(QString content, std::shared_ptr<Participant> sender, QDateTime timestamp);
    void addParticipant(std::shared_ptr<Participant> participant);
    void removeParticipant(std::shared_ptr<Participant> participant);
    void removeParticipant(ParticipantKey participantKey);
    std::vector<ParticipantKey> getParticipantKeys();

public slots:
    void add_message(Message msg);

signals:
    void messagesChanged();

protected:
    const ChatKey key;
    std::vector<std::shared_ptr<Participant>> participants;
};

#endif
