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

/**
 * @brief The Participant class represents the basic features of a participant in the chat program and can be used on client and server side.
 *
 * This class provides methods to get and set the participant's key, nickname, and entry date.
 * The participant's key is a unique identifier. On server side, this should be created using the UniqueKeyGenerator<ServerParticipant> class.
 * On client side, the client is informed about the participant's key by the server.
 * The nickname is the name by which the participant is known in the GUI.
 * The entry date is the date and time when the participant registered in the chat program. It is set to the current date and time by default.
 * On client side, the participant's entry date can be retrieved from the server.
 * The participant's key identifies the participant in the network communication.
 */
class Participant
{
public:
    /**
     * @brief Constructs a Participant object with the given key, name, and registeredSince values.
     *
     * @param key The participant's key.
     * @param name The participant's nickname (default: "Unknown").
     * @param registeredSince The participant's entry date (default: current date and time).
     */
    Participant(const ParticipantKey key,
                QString name = "Unknown",
                QDateTime registeredSince = QDateTime::currentDateTime()) : key(key), name(name), registeredSince(registeredSince){};

    /**
     * @brief Returns the participant's key.
     *
     * @return The participant's key.
     */
    ParticipantKey getKey() const { return key; }

    /**
     * @brief Returns the participant's nickname.
     *
     * @return The participant's nickname.
     */
    QString getNickname() const { return name; }

    /**
     * @brief Returns the participant's entry date.
     *
     * @return The participant's entry date.
     */
    QDateTime getEntryDate() const { return registeredSince; }

    /**
     * @brief Returns the participant's name.
     *
     * @return The participant's name as QString.
     */
    QString getNickname() const { return name; }

    /**
     * @brief Sets the participant's name.
     *
     * @param newName The new name for the participant.
     */
    void setNickname(QString newName) { name = newName; }

    /**
     * @brief Sets the participant's entry date.
     *
     * This method is probably only used on client side, when the participant's entry date was retrieved from the server.
     * It's not a hard requirement that this date is correct, so it is ok to show the date at creation time by default.
     *
     * @param entryDate The new entry date for the participant.
     */
    void setEntryDate(QDateTime entryDate)
    {
        registeredSince = entryDate;
    }

protected:
    const ParticipantKey key;  /**< The participant's key. */
    QString name;              /**< The participant's name. */
    QDateTime registeredSince; /**< The participant's entry date. */
};

#endif /* A23B1762_468C_4A2C_9B5B_74C56C1FEE08 */
