#include "chat.h"

#include "moc_chat.cpp"

#include <algorithm>

void Chat::add_message(Message msg)
{
    messages.emplace(msg);
    emit messagesChanged();
}

void Chat::foreach_do(std::function<void(const Message &)> func)
{
    std::for_each(messages.begin(), messages.end(), func);
}

const Message *Chat::get_last_message() const
{
    qDebug() << "Chat::get_last_message()";
    if (messages.empty())
    {
        qDebug() << "Chat::get_last_message() empty";
        return nullptr;
    }
    qDebug() << "Chat::get_last_message() not empty";
    return &(*messages.begin());
}