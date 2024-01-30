#include <QDateTime>
#include <QString>

#include "uniqueKey.h"
#include <cstdint>

typedef uint8_t ParticipantKey;

class Participant
{
        const ParticipantKey key;
        QString name;
        const QDateTime registered_since;

public:
        Participant() : key(UniqueKeyGenerator<ParticipantKey>::get_instance()->requestKey()), registered_since(QDateTime::currentDateTime())
        {
                name = "Unknown";
        };
        void setName(QString new_name) { name = new_name; };
        ParticipantKey getKey() const { return key; }
};
