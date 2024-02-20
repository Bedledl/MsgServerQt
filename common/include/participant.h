#ifndef A23B1762_468C_4A2C_9B5B_74C56C1FEE08
#define A23B1762_468C_4A2C_9B5B_74C56C1FEE08
#include <QDateTime>
#include <QString>
#include <cstdint>

typedef unsigned ParticipantKey;

class ParticipantNotFound : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Participant not found.";
    }
};

class ParticipantAlreadyExists : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Participant already exists.";
    }
};

class Participant
{
public:
    Participant(const ParticipantKey key,
                QString name = "Unknown",
                QDateTime registeredSince = QDateTime::currentDateTime()) : key(key), name(name), registeredSince(registeredSince){};
    ParticipantKey getKey() const { return key; }
    QString getNickname() const { return name; }
    QDateTime getEntryDate() const { return registeredSince; }
    QString getNickname() const { return name; }

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
