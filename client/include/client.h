#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QMap>
#include <QObject>

#include "chat.h"
#include "clientIface.h"
#include "communicator.h"
#include "participant.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpSocket;
QT_END_NAMESPACE

class Client : public QObject, public ClientIface
{
    Q_OBJECT
public:
    explicit Client(QHostAddress ip, quint16 port, QString nickname, bool pingMode, QObject *parent = nullptr);
    void addNewChat(const ChatKey &key) override
    {
        if (chats.contains(key))
        {
            return;
        }
        auto newChat = new Chat(key, this);
        chats.insert(key, newChat);
    }
    void leaveChat(const ChatKey &key) override
    {
        auto countDeleted = chats.remove(key);
        if (countDeleted == 0)
        {
            throw ChatNotFound();
        }
    }
    void addNewIncomingMessage(const ChatKey &key, QString content, const ParticipantKey &participantKey, QDateTime timestamp) override
    {
        if (auto searchChat = chats.find(key); searchChat != chats.end())
        {
            if (auto searchParticipant = registeredParticipants.find(participantKey); searchParticipant != registeredParticipants.end())
            {
                (*searchChat)->addMessage(std::move(content), *searchParticipant, std::move(timestamp));
            }
            else
            {
                throw ParticipantNotFound();
            }
        }
        else
        {
            throw ChatNotFound();
        }
    }

    void addParticipant(const ParticipantKey &key) override
    {
        if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
        {
            throw ParticipantAlreadyExists();
        }
        registeredParticipants.insert(key, std::make_shared<Participant>(key));
    }

    void removeParticipant(const ParticipantKey &key) override
    {
        auto nrDeleted = registeredParticipants.remove(key);
        if (nrDeleted == 0)
        {
            throw ParticipantNotFound();
        }
    }

    void assignParticipantName(const ParticipantKey &key, QString name) override
    {
        if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
        {
            (*search)->setNickname(name);
        }
    }
    void assignParticipantEntryDate(const ParticipantKey &key, QDateTime entryDate) override
    {
        if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
        {
            (*search)->setEntryDate(std::move(entryDate));
        }
    }
    void addParticipantToChat(const ChatKey &chatKey, const ParticipantKey &participantKey)
    {
        if (auto searchChat = chats.find(chatKey); searchChat != chats.end())
        {
            if (auto searchParticipant = registeredParticipants.find(participantKey); searchParticipant != registeredParticipants.end())
            {
                (*searchChat)->addParticipant(*searchParticipant);
            }
            else
            {
                throw ParticipantNotFound();
            }
        }
        else
        {
            throw ChatNotFound();
        }
    }
    void removeParticipantFromChat(const ChatKey &chatKey, const ParticipantKey &participantKey)
    {
        if (auto searchChat = chats.find(chatKey); searchChat != chats.end())
        {
            if (auto searchParticipant = registeredParticipants.find(participantKey); searchParticipant != registeredParticipants.end())
            {
                (*searchChat)->removeParticipant(*searchParticipant);
            }
            else
            {
                throw ParticipantNotFound();
            }
        }
        else
        {
            throw ChatNotFound();
        }
    }
    QString getNickname() const override { return nickname; }

private slots:
    void readFromSocketAndAswer();

private:
    QString nickname;
    QLabel *statusLabel = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QByteArray block;
    QDataStream out{&block, QIODevice::WriteOnly};
    std::unique_ptr<Communicator> communicator;
    QMap<ChatKey, Chat *> chats;
    QMap<ParticipantKey, std::shared_ptr<Participant>> registeredParticipants;
};

#endif
