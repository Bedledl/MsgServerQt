#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractListModel>
#include <QAbstractSocket>
#include <QMap>
#include <QObject>
#include <qqml.h>

#include "chat.h"
#include "chatPreviewModel.h"
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
    Q_PROPERTY(QString nickname READ getNickname CONSTANT)
    Q_PROPERTY(int port READ getPort CONSTANT)
    Q_PROPERTY(QString ip READ getIp CONSTANT)
    Q_PROPERTY(ChatPreviewListModel *CPLmodel READ getChatPreviewListModel CONSTANT)
public:
    explicit Client(QHostAddress ip, quint16 port, bool pingMode, QObject *parent = nullptr);
    void addChat(const ChatKey &key)
    {
        qDebug() << "Client::addChat";
        if (chats.contains(key))
        {
            qDebug() << "Chat already exists";
            return;
        }
        auto newChat = new Chat(key, this);
        chats.insert(key, newChat);
        chatPreviewListModel->addChat(newChat);
    }
    void leaveChat(const ChatKey &key)
    {
        auto chat = chats.take(key);
        delete chat;
        // TODO
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
    QString getNickname() const;
    int getPort() const;
    QString getIp() const;
    auto getChatPreviewListModel() { return chatPreviewListModel; };
signals:
    void chatPreviewListChanged();

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
    QString nickname;
    QString remoteIpString;
    quint16 remotePort;
    ChatPreviewListModel *chatPreviewListModel;
};

#endif