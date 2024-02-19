#ifndef A81E4559_7B19_43E2_AD18_B06CF2B4A3EE
#define A81E4559_7B19_43E2_AD18_B06CF2B4A3EE
#include <QAbstractListModel>
#include <QDateTime>
#include <QModelIndex>
#include <QObject>

#include "chat.h"

// TODO enhancement sorting with https://doc.qt.io/qt-6/qsortfilterproxymodel.html
// currently just show chats
// further enhancements: read and unread messages

class ChatPreviewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastMessageText READ lastMessage CONSTANT);
    Q_PROPERTY(QDateTime lastMessageTimestamp READ lastMessageTimestamp CONSTANT);
    Q_PROPERTY(QString chatName READ chatName CONSTANT);

public:
    ChatPreviewModel(Chat *chat, QObject *parent = nullptr) : QObject(parent), chat(chat)
    {
        if (chat == nullptr)
        {
            throw std::invalid_argument("ChatPreviewModel::ChatPreviewModel: chat is nullptr");
        }
    };

private:
    Chat *chat;
    QString chatName() const;
    QString lastMessage() const;
    QDateTime lastMessageTimestamp() const;

    friend class ChatPreviewListModel;
};

class ChatPreviewListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int selectedChat READ selectedChat WRITE setSelectedChat NOTIFY selectedChatChanged);
public:
    void addChat(Chat *chat);
    void removeChat(Chat *chat);
    ChatPreviewListModel(QMap<ChatKey, Chat *> *chats, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int selectedChat() const { return selectedChatIndex; };
    void setSelectedChat(int selectedChat)
    {
        selectedChatIndex = selectedChat;
        emit selectedChatChanged(selectedChat);
    };
signals:
    void selectedChatChanged(int selectedChat);

private:
    QList<ChatPreviewModel *> chatsPreviews = QList<ChatPreviewModel *>();
    int selectedChatIndex = -1;
};

#endif /* A81E4559_7B19_43E2_AD18_B06CF2B4A3EE */
