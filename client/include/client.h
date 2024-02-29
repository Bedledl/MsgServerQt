#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QMap>
#include <QObject>
#include <memory>

#include "chat.h"
#include "clientIface.h"
#include "communicator.h"
#include "chatPreviewModel.h"
#include "participant.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QTcpSocket;
QT_END_NAMESPACE

class Client : public QObject, public ClientIface
{
    Q_OBJECT
    Q_PROPERTY(QString nickname READ getNickname CONSTANT)
    Q_PROPERTY(int port READ getPort CONSTANT)
    Q_PROPERTY(QString ip READ getIp CONSTANT)
    Q_PROPERTY(ChatPreviewListModel *CPLmodel READ getChatPreviewListModel CONSTANT)
    Q_PROPERTY(int selectedChat READ selectedChat WRITE setSelectedChat NOTIFY selectedChatChanged);

public:
    explicit Client(QHostAddress ip, quint16 port, QString nickname, bool pingMode, QObject *parent = nullptr);
    void addNewChat(const ChatKey &key) override;
    void leaveChat(const ChatKey &key) override;
    void addNewIncomingMessage(const ChatKey &key, QString content, const ParticipantKey &participantKey, QDateTime timestamp) override;
    void addParticipant(const ParticipantKey &key) override;
    void removeParticipant(const ParticipantKey &key) override;
    void assignParticipantName(const ParticipantKey &key, QString name) override;
    void assignParticipantEntryDate(const ParticipantKey &key, QDateTime entryDate) override;
    void addParticipantToChat(const ChatKey &chatKey, const ParticipantKey &participantKey) override;
    void removeParticipantFromChat(const ChatKey &chatKey, const ParticipantKey &participantKey) override;
    QString getNickname() const override;
    bool participantIsRegistered(const ParticipantKey &key) const override;
    int getPort() const;
    QString getIp() const;
    auto getChatPreviewListModel() { return chatPreviewListModel.get(); };
    int selectedChat() const { return selectedChatIndex; };
    void setSelectedChat(int selectedChat)
    {
        selectedChatIndex = selectedChat;
        emit selectedChatChanged(selectedChat);
    };
signals:
    void selectedChatChanged(int selectedChat);
    void chatPreviewListChanged();

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
    QString remoteIpString;
    quint16 remotePort;
    std::unique_ptr<ChatPreviewListModel> chatPreviewListModel;
    int selectedChatIndex = -1;
};

#endif
