#include "chat.h"

#include "moc_chat.cpp"

void Chat::add_message(Message msg)
{
    messages.emplace(msg);
    emit messagesChanged();
}

void Chat::foreach_do(std::function<void(const Message &)> func)
{
    std::for_each(messages.begin(), messages.end(), func);
}