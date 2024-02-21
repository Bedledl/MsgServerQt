#ifndef CHAT_H
#define CHAT_H

#include <QDate>
#include <QObject>
#include <QReadWriteLock>
#include <functional>
#include <set>
#include <memory>

#include "message.h"

class Participant;

typedef unsigned ChatKey;

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

public slots:
    void add_message(Message msg);

signals:
    void messagesChanged();

protected:
    const ChatKey key;
};

#endif
