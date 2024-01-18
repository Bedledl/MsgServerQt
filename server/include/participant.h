#include <QDateTime>
#include <QString>

#include "uniqueKey.h"

class Participant
{
        const UniqueKey<Participant> key;
        QString name;
        const QDateTime registered_since;

public:
        Participant() : registered_since(QDateTime::currentDateTime())
        {
                name = "Unknown";
        };
        void setName(QString new_name) { name = new_name; };
        unsigned getKey() const { return key.getKey(); }
};
