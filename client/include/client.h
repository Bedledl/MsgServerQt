#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QMap>
#include <QObject>

#include "chat.h"
#include "participant.h"

class Communicator;

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpSocket;
QT_END_NAMESPACE

class ClientFailedToConnect : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Failed to connect.";
    }
};

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QHostAddress ip, quint16 port, QObject *parent = nullptr);
    void addChat(const ChatKey &key) { chats.insert(key, new Chat(key, this)); }
    void leaveChat(const ChatKey &key)
    {
        auto chat = chats.take(key);
        delete chat;
    }
    void addNewMessage(const ChatKey &key, QString content, uint8_t participantKey, QDateTime timestamp)
    {
        if (auto search = chats.find(key); search != chats.end())
        {
            auto chat_ptr = *search;
            chat_ptr->addMessage(content, participantKey, timestamp);
        }
    }
    void assignParticipantName(const ParticipantKey &key, QString name)
    {
        if (auto search = otherParticipants.find(key); search != otherParticipants.end())
        {
            search->setNickname(name);
        }
    }
    void assignParticipantEntryDate(const ParticipantKey &key, QDateTime entryDate)
    {
        if (auto search = otherParticipants.find(key); search != otherParticipants.end())
        {
            search->setEntryDate(entryDate);
        }
    }

private slots:
    void readFromSocketAndAswer();

private:
    QLabel *statusLabel = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
    Communicator *communicator;
    QMap<ChatKey, Chat *> chats;
    QMap<ParticipantKey, Participant> otherParticipants;
};

#endif