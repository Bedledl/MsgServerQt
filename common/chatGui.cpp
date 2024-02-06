#include "chatGui.h"
#include "chat.h"
#include "moc_chatGui.cpp"
#include <iostream>

void ChatGui::chatsChanged()
{
    message_texts.clear();
    if (chat)
    {
        chat->foreach_do([&](const Message &msg)
                         { message_texts.append(new QLabel(msg.toString())); });
    }
}

void ChatGui::updateMessages()
{
}

ChatGui::ChatGui(QWidget *parent, Chat *chat) : QWidget(parent), chat(chat)
{
    std::cout << "Chat Gui Constructor" << std::endl;
}