#include <algorithm>

#include "chat.h"
#include "participant.h"

#include "moc_chat.cpp"

void Chat::foreach_do(std::function<void(const Message &)> func)
{
    mutex.lockForRead();
    std::for_each(messages.begin(), messages.end(), func);
    mutex.unlock();
}

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
    if (std::find(participants.begin(), participants.end(), participant) != participants.end())
    {
        mutex.unlock();
        throw ParticipantAlreadyExists();
    }
    participants.emplace_back(std::move(participant));
    mutex.unlock();
};
void Chat::removeParticipant(std::shared_ptr<Participant> participant)
{
    mutex.lockForWrite();
    auto countErased = std::erase(participants, participant);
    if (countErased == 0)
    {
        mutex.unlock();
        throw ParticipantNotFound();
    }
    mutex.unlock();
};
void Chat::removeParticipant(ParticipantKey participantKey)
{
    auto countErased = std::erase_if(participants, [participantKey](std::shared_ptr<Participant> &participant)
                                     { return participant->getKey() == participantKey; });
    if (countErased == 0)
    {
        throw ParticipantNotFound();
    }
};

std::vector<ParticipantKey> Chat::getParticipantKeys()
{
    std::vector<ParticipantKey> keys;
    std::transform(participants.begin(), participants.end(), std::back_inserter(keys), [](std::shared_ptr<Participant> &participant)
                   { return participant->getKey(); });
    return keys;
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

