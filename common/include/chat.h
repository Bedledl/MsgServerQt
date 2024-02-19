#ifndef CHAT_H
#define CHAT_H

#include <QDate>
#include <QObject>
#include <QReadWriteLock>
#include <functional>
#include <set>

#include "message.h"
#include "participant.h"

typedef unsigned ChatKey;

class Chat : public QObject
{
    Q_OBJECT

public:
    Chat(ChatKey key, QObject *parent = nullptr) : QObject(parent), key(key)
    {
        messages = std::set<Message>();
    };
    void foreach_do(std::function<void(const Message &)> func);
    const Message *get_last_message() const;
    ChatKey getKey() const { return key; };
    void addMessage(QString content, uint8_t participantKey, QDateTime timestamp)
    {
        participantKey;
        timestamp;
        // TODO
        add_message(Message(content, nullptr));
    }
    void addParticipant(Participant *participant)
    {
        participants.append(participant);
    }
    void removeParticipant(Participant *participant)
    {
        participants.removeOne(participant);
    }

public slots:
    void add_message(Message msg);

signals:
    void messagesChanged();

protected:
    const ChatKey key;
    std::set<Message> messages;
    QList<Participant *> participants = QList<Participant *>();
};

#endif