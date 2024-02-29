#include <algorithm>

#include "chat.h"
#include "participant.h"

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

void Chat::addMessage(QString content, std::shared_ptr<Participant> sender, QDateTime timestamp)
{
    sender;
    timestamp;
    // TODO
    qDebug() << "Chat: add Message ";
    throw std::runtime_error("Not implemented");
    add_message(Message(content));
}
void Chat::addParticipant(std::shared_ptr<Participant> participant)
{
    if (std::find(participants.begin(), participants.end(), participant) != participants.end())
    {
        throw ParticipantAlreadyExists();
    }
    participants.emplace_back(std::move(participant));
};
void Chat::removeParticipant(std::shared_ptr<Participant> participant)
{
    auto countErased = std::erase(participants, participant);
    if (countErased == 0)
    {
        throw ParticipantNotFound();
    }
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

