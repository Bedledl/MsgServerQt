#include <QHostAddress>
#include <QString>
#include <utility>

#include "chat.h"
#include "client.h"
#include "clientMessageBuilder.h"
#include "communicator.h"
#include "rawMessageProcessor.h"
#include "tcpClientConnector.h"

#include "moc_client.cpp"

Client::~Client() = default;

Client::Client(const QHostAddress &ip, quint16 port, const QString &nickname, bool pingMode, QObject *parent)
    : QObject(parent), nickname(nickname)
{
    if (pingMode)
    {
        auto pingCommunicator = std::make_shared<PingPongCommunicator>([this](std::string msg)
                                                                       { tcpClientConnector->sendMsgToServer(std::move(msg)); });

        auto incomingMessageCb = [pingCommunicator](std::string rawMsg)
        {
            pingCommunicator->processRawMessage(std::move(rawMsg));
        };

        tcpClientConnector = std::make_unique<TcpClientConnector>(
            ip, port, incomingMessageCb,
            this);
        return;
    }

    tcpClientConnector = std::make_unique<TcpClientConnector>(
        ip, port, [&](std::string rawMsg)
        { rawMessageProcessor->processRawMessage(std::move(rawMsg)); },
        this);

    rawMessageProcessor = std::make_unique<RawMessageProcessor>(*this, [this](std::string msg)
                                                                { tcpClientConnector->sendMsgToServer(std::move(msg)); });

    tcpClientConnector->sendMsgToServer(ClientMessageBuilder::getSetNewNickname(0, nickname.toStdString()));

    // chatPreviewListModel = std::make_unique<ChatPreviewListModel>(&chats, this);
}
void Client::addNewChat(const SequenceNumber /*seqNr*/, const ChatKey &key)
{
    if (chats.contains(key))
    {
        return;
    }
    auto *newChat = new Chat(key, this);
    chats.insert(key, newChat);
    // chatPreviewListModel->addChat(newChat);
}

void Client::leaveChat(const SequenceNumber /*seqNr*/, const ChatKey &key)
{
    auto countDeleted = chats.remove(key);
    if (countDeleted == 0)
    {
        throw ChatNotFound();
    }
}

void Client::addNewIncomingMessage(const SequenceNumber /*seqNr*/, const ChatKey &key, QString content, const ParticipantKey &participantKey, QDateTime timestamp)
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

void Client::addParticipant(const SequenceNumber /*seqNr*/, const ParticipantKey &key)
{
    if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
    {
        throw ParticipantAlreadyExists();
    }
    registeredParticipants.insert(key, std::make_shared<Participant>(key));
}

void Client::removeParticipant(const SequenceNumber /*seqNr*/, const ParticipantKey &key)
{
    auto nrDeleted = registeredParticipants.remove(key);
    if (nrDeleted == 0)
    {
        throw ParticipantNotFound();
    }
}

void Client::assignParticipantName(const SequenceNumber /*seqNr*/, const ParticipantKey &key, QString name)
{
    if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
    {
        (*search)->setNickname(name);
    }
}

void Client::assignParticipantEntryDate(const SequenceNumber /*seqNr*/, const ParticipantKey &key, QDateTime entryDate)
{
    if (auto search = registeredParticipants.find(key); search != registeredParticipants.end())
    {
        (*search)->setEntryDate(std::move(entryDate));
    }
}

void Client::addParticipantToChat(const SequenceNumber /*seqNr*/, const ChatKey &chatKey, const ParticipantKey &participantKey)
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

void Client::removeParticipantFromChat(const SequenceNumber /*seqNr*/, const ChatKey &chatKey, const ParticipantKey &participantKey)
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

void Client::sendNewMsg(const SequenceNumber seqNr, ChatKey chatKey, std::string msg)
{
    tcpClientConnector->sendMsgToServer(ClientMessageBuilder::getSendNewMsg(seqNr, chatKey, std::move(msg)));
}

void Client::receivedMalformedMessage(const SequenceNumber seqNr) const
{
    auto malformedMessageAnswer = ClientMessageBuilder::getGenericResponseMalformed(seqNr);
    tcpClientConnector->sendMsgToServer(malformedMessageAnswer);
}
