#ifndef CLIENT_H
#define CLIENT_H
#include <QAbstractSocket>
#include <QMap>
#include <QObject>
#include <memory>

#include "clientIface.h"
#include "uniqueKeysTypedef.h"
// #include "chatPreviewListModel.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpSocket;
QT_END_NAMESPACE

class TcpClientConnector;
class RawMessageProcessor;
class Chat;
class Participant;

class Client : public QObject, public ClientOuterIface, public ClientInnerIface
{
    Q_OBJECT
    // Q_PROPERTY(QString nickname READ getNickname CONSTANT)
    // Q_PROPERTY(int port READ[]() { return tcpClientConnector.getRemotePort(); } CONSTANT)
    // Q_PROPERTY(QString ip READ[]() { return tcpClientConnector.getRemoteIpString(); } CONSTANT)
    // Q_PROPERTY(ChatPreviewListModel *CPLmodel READ getChatPreviewListModel CONSTANT)
    // Q_PROPERTY(ChatViewModel *CVmodel READ getChatViewModel CONSTANT)
    // Q_PROPERTY(int selectedChat READ selectedChat WRITE setSelectedChat NOTIFY selectedChatChanged);

public:
    explicit Client(const QHostAddress &ip, quint16 port, const QString &nickname, bool pingMode, QObject *parent = nullptr);
    ~Client() override;
    void addNewChat(SequenceNumber seqNr, const ChatKey &key) override;
    void leaveChat(SequenceNumber seqNr, const ChatKey &key) override;
    void addNewIncomingMessage(SequenceNumber seqNr, const ChatKey &key, QString content, const ParticipantKey &participantKey, QDateTime timestamp) override;
    void addParticipant(SequenceNumber seqNr, const ParticipantKey &key) override;
    void removeParticipant(SequenceNumber seqNr, const ParticipantKey &key) override;
    void assignParticipantName(SequenceNumber seqNr, const ParticipantKey &key, QString name) override;
    void assignParticipantEntryDate(SequenceNumber seqNr, const ParticipantKey &key, QDateTime entryDate) override;
    void addParticipantToChat(SequenceNumber seqNr, const ChatKey &chatKey, const ParticipantKey &participantKey) override;
    void removeParticipantFromChat(SequenceNumber seqNr, const ChatKey &chatKey, const ParticipantKey &participantKey) override;
    void receivedMalformedMessage(SequenceNumber seqNr) const override;

    [[nodiscard]] bool participantIsRegistered(const ParticipantKey &key) const override;
    [[nodiscard]] QString getNickname() const override { return nickname; };
    void sendNewMsg(SequenceNumber seqNr, ChatKey chatKey, std::string msg) override;

    // auto getChatPreviewListModel() { return chatPreviewListModel.get(); };
    // ChatViewModel *getChatViewModel()
    // {
    //     if (selectedChatIndex != -1)
    //     {
    //         auto it = chats.begin();
    //         std::advance(it, selectedChatIndex);
    //         return new ChatViewModel(*(it.value()), *this, this);
    //     }
    //     return nullptr;
    // };
    // int selectedChat() const { return selectedChatIndex; };
    // void setSelectedChat(int selectedChat)
    // {
    //     selectedChatIndex = selectedChat;
    //     emit selectedChatChanged(selectedChat);
    // };

    // signals:
    //     void selectedChatChanged(int selectedChat);
    //     void chatPreviewListChanged();

private:
    QString nickname;
    std::unique_ptr<TcpClientConnector> tcpClientConnector;
    std::unique_ptr<RawMessageProcessor> rawMessageProcessor;
    QMap<ChatKey, Chat *> chats;
    QMap<ParticipantKey, std::shared_ptr<Participant>> registeredParticipants;

    // QLabel *statusLabel = nullptr;
    //  std::unique_ptr<ChatPreviewListModel> chatPreviewListModel;
    // int selectedChatIndex = -1;
};

#endif
