#ifndef CHAT_GUI_H
#define CHAT_GUI_H

#include "chat.h"
#include <QLabel>
#include <QVector>
#include <QWidget>
#include <memory>

// if chat is closed may cease membership in chat

class ChatGui : public QWidget
{
    Q_OBJECT

    // TODO: use other connection maybe Signal/Slots maybe weak pointer
    // don't use shared or unique, because chats should be only owned by ClientThreadWorkers
    Chat *chat;
    QVector<QLabel *> message_texts;

public:
    ChatGui(QWidget *parent, Chat *chat);

public slots:
    void chatsChanged();

private:
    void updateMessages();
};

#endif
