#ifndef ADFA9B81_CD9E_4260_ABAD_8AD7EE71BE2D
#define ADFA9B81_CD9E_4260_ABAD_8AD7EE71BE2D

#include <QDateTime>
#include <QString>

#include "uniqueKeysTypedef.h"

class ClientFailedToConnect : public std::exception
{
public:
    [[nodiscard]] const char *what() const noexcept override
    {
        return "Failed to connect.";
    }
};

class ClientOuterIface
{
public:
    virtual void addNewChat(SequenceNumber seqNr, const ChatKey &key) = 0;
    virtual void leaveChat(SequenceNumber seqNr, const ChatKey &key) = 0;
    virtual void addNewIncomingMessage(SequenceNumber seqNr, const ChatKey &key, QString content, const ParticipantKey &participantKey, QDateTime timestamp) = 0;
    virtual void addParticipant(SequenceNumber seqNr, const ParticipantKey &key) = 0;
    virtual void removeParticipant(SequenceNumber seqNr, const ParticipantKey &key) = 0;
    virtual void assignParticipantName(SequenceNumber seqNr, const ParticipantKey &key, QString name) = 0;
    virtual void assignParticipantEntryDate(SequenceNumber seqNr, const ParticipantKey &key, QDateTime entryDate) = 0;
    virtual void addParticipantToChat(SequenceNumber seqNr, const ChatKey &chatKey, const ParticipantKey &participantKey) = 0;
    virtual void removeParticipantFromChat(SequenceNumber seqNr, const ChatKey &chatKey, const ParticipantKey &participantKey) = 0;
    virtual void receivedMalformedMessage(SequenceNumber seqNr) const = 0;
};

class ClientInnerIface
{
public:
    [[nodiscard]] virtual QString getNickname() const = 0;
    virtual void sendNewMsg(SequenceNumber seqNr, ChatKey chatKey, std::string msg) = 0;
    [[nodiscard]] virtual bool participantIsRegistered(const ParticipantKey &key) const = 0;
};

#endif /* ADFA9B81_CD9E_4260_ABAD_8AD7EE71BE2D */
