#ifndef A23B1762_468C_4A2C_9B5B_74C56C1FEE08
#define A23B1762_468C_4A2C_9B5B_74C56C1FEE08
#include <QDateTime>
#include <QString>
#include <cstdint>

typedef uint8_t ParticipantKey;

class Participant
{
public:
    Participant(const ParticipantKey key) : key(key)
    {
        name = "Unknown";
        registeredSince = QDateTime::currentDateTime();
    };
    ParticipantKey getKey() const { return key; }

    void setNickname(QString name) { name = name; }
    void setEntryDate(QDateTime entryDate)
    {
        registeredSince = entryDate;
    }

protected:
    const ParticipantKey key;
    QString name;
    QDateTime registeredSince;
};

#endif /* A23B1762_468C_4A2C_9B5B_74C56C1FEE08 */
