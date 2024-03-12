#include <QDateTime>
#include <QString>

#include "common/include/participant.h"
#include "uniqueKey.h"

class ServerParticipant : public Participant
{

public:
    ServerParticipant() : Participant(UniqueKeyGenerator<ParticipantKey>::get_instance()->requestKey())
    {
        registeredSince = QDateTime::currentDateTime();
        name = "Unknown";
    };
};
