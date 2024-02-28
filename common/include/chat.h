#ifndef CHAT_H
#define CHAT_H

#include <QDate>
#include <QObject>
#include <QReadWriteLock>
#include <functional>
#include <memory>
#include <set>

#include "message.h"

class Participant;

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
    void addMessage(QString content, std::shared_ptr<Participant> sender, QDateTime timestamp)
    {
        sender;
        timestamp;
        // TODO
        add_message(Message(content));
    }
    void addParticipant(std::shared_ptr<Participant> participant)
    {
        // TODO
        qWarning("Chat::addParticipant not implemented");
    };
    void removeParticipant(std::shared_ptr<Participant> participant)
    {
        // TODO
        qWarning("Chat::removeParticipant not implemented");
    };

public slots:
    void add_message(Message msg);

signals:
    void messagesChanged();

protected:
    const ChatKey key;
    std::vector<std::shared_ptr<Participant>> participants;
};

#endif
