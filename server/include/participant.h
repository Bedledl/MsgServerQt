#include <QDateTime>
#include <QString>

#include "uniqueKey.h"

typedef uint8_8 ParticipantKey;

class Participant
{
        const ParticipantKey key;
        QString name;
        const QDateTime registered_since;

public:
        Participant() : registered_since(QDateTime::currentDateTime()), key(UniqueKeyGenerator<ParticipantKey>.get_instance()->requestKey())
        {
                name = "Unknown";
        };
        void setName(QString new_name) { name = new_name; };
        ParticipantKey getKey() const { return key; }
};
