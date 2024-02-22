#ifndef ADFA9B81_CD9E_4260_ABAD_8AD7EE71BE2D
#define ADFA9B81_CD9E_4260_ABAD_8AD7EE71BE2D

#include <QDateTime>
#include <QString>

typedef unsigned ParticipantKey;
typedef unsigned ChatKey;

class ClientFailedToConnect : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Failed to connect.";
    }
};

class ParticipantNotFound : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Participant not found.";
    }
};

class ChatNotFound : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Chat not found.";
    }
};

class ChatAlreadyExists : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Chat already exists.";
    }
};

class ClientIface
{
public:
    virtual void addNewChat(const ChatKey &key) = 0;
    virtual void leaveChat(const ChatKey &key) = 0;
    virtual void addNewIncomingMessage(const ChatKey &key, QString content, const ParticipantKey &participantKey, QDateTime timestamp) = 0;
    virtual void assignParticipantName(const ParticipantKey &key, QString name) = 0;
    virtual void assignParticipantEntryDate(const ParticipantKey &key, QDateTime entryDate) = 0;
    virtual QString getNickname() const = 0;
};

#endif /* ADFA9B81_CD9E_4260_ABAD_8AD7EE71BE2D */
