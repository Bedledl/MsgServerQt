#ifndef AC220DE2_7603_4C5F_B431_BA72113764DD
#define AC220DE2_7603_4C5F_B431_BA72113764DD
#include <QDateTime>

#include "chat.h"
#include "participant.h"

class ServerIface
{
public:
    virtual void removeParticipantFromChat(ChatKey key, ParticipantKey participantKey) const = 0;
    virtual void addParticipantToChat(std::shared_ptr<ServerChat> chat, ParticipantKey participantKey) const = 0;
    virtual std::string getParticipantName(ParticipantKey key) const = 0;
    virtual void setParticipantName(ParticipantKey key, std::string name) const = 0;
    virtual QDateTime getParticipantEntryDate(ParticipantKey key) const = 0;
    virtual void setParticipantEntryDate(ParticipantKey key, QDateTime entryDate) const = 0;
    virtual void addNewIncomingMessage(ChatKey key, std::string content, ParticipantKey participantKey, QDateTime timestamp) const = 0;
};

#endif /* AC220DE2_7603_4C5F_B431_BA72113764DD */
