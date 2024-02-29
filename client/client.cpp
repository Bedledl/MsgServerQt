#include "include/client.h"
#include "include/chatPreviewModel.h"
#include "include/communicators.h"

#include <QDebug>
#include <QIODevice>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <memory>
#include <qqml.h>

#include <iostream>

#include "moc_client.cpp"

Client::Client(QHostAddress ip, quint16 port, QString nickname, bool pingMode, QObject *parent)
    : QObject(parent), tcpSocket(new QTcpSocket(this)), nickname(nickname)
{
    std::cout << "client constrctor" << std::endl;

    if (pingMode)
    {
        qDebug() << "Creating PingPongCommunicator";
        communicator = std::make_unique<PingPongCommunicator>();
    }
    else
    {
        qDebug() << "Creating ClientCommunicator";
        communicator = std::make_unique<ClientCommunicator>(*this);
    }
    tcpSocket->connectToHost(ip, port);
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_6_6);
    out.setVersion(QDataStream::Qt_6_6);
    std::cout << "QDataStream Status: " << in.status() << std::endl;
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readFromSocketAndAswer);
    connect(tcpSocket, &QAbstractSocket::errorOccurred,
            //! [3]
            [&](QAbstractSocket::SocketError socketError)
            { qDebug() << tcpSocket->errorString(); });
    out << communicator->welcomeMessage();
    nickname = "Juliet";
    remoteIpString = ip.toString();
    remotePort = port;
    chatPreviewListModel = std::make_unique<ChatPreviewListModel>(&chats, this);
}

void Client::readFromSocketAndAswer()
{
    std::cout << "read from Socket begin" << std::endl;
    std::cout << "QDataStream Status: " << in.status() << std::endl;

    // auto r = tcpSocket->readAll();
    // qDebug() << QString::fromLatin1(r);

    in.startTransaction();
    if (!in.commitTransaction())
    {
        std::cout << "Error QDataStream Status: " << in.status() << std::endl;
        in.abortTransaction();
        return;
    }
    QString msg;
    in >> msg;

    std::cout << "read from socket and answer" << std::endl;

    if (msg.length() == 0)
    {
        qDebug() << "empty message";
        return;
    }

    qDebug() << msg;

    auto answer = communicator->answerMessage(msg);
    qDebug() << answer;

    if (answer.length() == 0)
    {
        qDebug() << "empty answer, won't send it";
        return;
    }

    out << answer;

    qDebug() << "block: " << block;

    auto written = tcpSocket->write(block);

    std::cout << "wrote Message to socket with length: " << written << std::endl;
}

void Client::addNewChat(const ChatKey &key)
{
    if (chats.contains(key))
    {
        return;
    }
    auto newChat = new Chat(key, this);
    chats.insert(key, newChat);
    chatPreviewListModel->addChat(newChat);
}
void Client::leaveChat(const ChatKey &key)
{
    auto countDeleted = chats.remove(key);
    if (countDeleted == 0)
    {
        throw ChatNotFound();
    }
}
void Client::addNewIncomingMessage(const ChatKey &key, QString content, const ParticipantKey &participantKey, QDateTime timestamp)
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
void Client::addParticipant(const ParticipantKey &key)
{
    if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
    {
        throw ParticipantAlreadyExists();
    }
    registeredParticipants.insert(key, std::make_shared<Participant>(key));
}
void Client::removeParticipant(const ParticipantKey &key)
{
    auto nrDeleted = registeredParticipants.remove(key);
    if (nrDeleted == 0)
    {
        throw ParticipantNotFound();
    }
}
void Client::assignParticipantName(const ParticipantKey &key, QString name)
{
    if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
    {
        (*search)->setNickname(name);
    }
}
void Client::assignParticipantEntryDate(const ParticipantKey &key, QDateTime entryDate)
{
    if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
    {
        (*search)->setEntryDate(std::move(entryDate));
    }
}
void Client::addParticipantToChat(const ChatKey &chatKey, const ParticipantKey &participantKey)
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
void Client::removeParticipantFromChat(const ChatKey &chatKey, const ParticipantKey &participantKey)
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

bool Client::participantIsRegistered(const ParticipantKey &key) const
{
    return registeredParticipants.contains(key);
}

inline QString Client::getNickname() const { return nickname; }
inline int Client::getPort() const { return remotePort; }
inline QString Client::getIp() const { return remoteIpString; }
