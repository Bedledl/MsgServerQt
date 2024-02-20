#include <sstream>
#include <stdexcept>

#include "chat.h"

#include "moc_chat.cpp"

#include <algorithm>

void Chat::addMessage(Message msg)
{
    mutex.lockForRead();
    auto sender = msg.getSender();
    if (!participants.contains(sender))
    {
        mutex.unlock();
        throw SenderNotChatParticipant();
    }

    if (messages.contains(msg))
    {
        mutex.unlock();
        return;
    }

    bool messageWasAppended = false;
    if (messages.size() == 0 || msg > getLastMessage())
    {
        messageWasAppended = true;
    }

    mutex.unlock();

    mutex.lockForWrite();
    messages.emplace(msg);
    mutex.unlock();
    if (messageWasAppended)
    {
        emit messageAppended();
    }
    else
    {
        emit messageOrderInvalidated();
    }
}
void Chat::addMessage(QString content, std::shared_ptr<Participant> sender, QDateTime timestamp = QDateTime::currentDateTime())
{
    addMessage(Message(content, sender, timestamp));
}

void Chat::addParticipant(std::shared_ptr<Participant> participant)
{
    mutex.lockForWrite();
    if (!participants.contains(participant))
    {
        participants.append(participant);
    }
    mutex.unlock();
}

void Chat::removeParticipant(std::shared_ptr<Participant> participant)
{
    mutex.lockForWrite();
    participants.removeOne(participant);
    mutex.unlock();
}

void Chat::foreach_do(std::function<void(const Message &)> func)
{
    mutex.lockForRead();
    std::for_each(messages.begin(), messages.end(), func);
    mutex.unlock();
}

const Message &Chat::getLastMessage() const
{
    mutex.lockForRead();
    qDebug() << "Chat::get_last_message()";
    if (messages.empty())
    {
        mutex.unlock();
        throw std::out_of_range("No messages in chat.");
    }
    mutex.unlock();
    return getMessageAt(messages.size() - 1);
}

const Message &Chat::getMessageAt(size_t index) const
{
    mutex.lockForRead();
    if (index < 0 || index >= messages.size())
    {
        mutex.unlock();
        std::stringstream ss;
        ss << "Message index " << index << " out of bounds.";
        throw std::out_of_range(ss.str());
    }
    auto &r = *std::next(messages.begin(), index);
    mutex.unlock();
    return r;
}