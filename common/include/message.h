#ifndef MESSAGE_H
#define MESSAGE_H

#include <chrono>
#include <compare>
#include <string>

#include <QDateTime>

/// @brief  forward declaration of Participant
class Participant;

/**
 * @brief The Message class is a basic representation of a message in the chat program. It can be used on server and client side.
 *
 * It contains information about the content, sender, and timestamp of the message.
 * It is not intended to change any data within the message once it is created.
 */
class Message

{
public:
    /**
     * @brief Constructs a Message object with the given content, sender, and timestamp.
     *
     * @param content The content of the message.
     * @param sender The participant who sent the message.
     * @param sentDateTime The timestamp when the message was sent.
     */
    Message(QString content, std::shared_ptr<Participant> sender, QDateTime sentDateTime = QDateTime::currentDateTime());

    /**
     * @brief Compares two Message objects based on their timestamps and content.
     * This is especially important for ordering the messages based on their sent date in a chat.
     *
     * @param msg The Message object to compare with.
     * @return A strong ordering result indicating the relative order of the messages.
     */
    std::strong_ordering operator<=>(const Message &msg) const;

    /**
     * @brief Returns the content of the message.
     *
     * @return The content of the message.
     */
    [[nodiscard]] QString getContent() const;

    /**
     * @brief Returns a string representation of the message. This string representation contains the formatted timestamp
     * at which the message was sent the name of the sender and the content of the message. Its primary use case is debugging.
     *
     * @return A string representation of the message.
     */
    [[nodiscard]] QString toString() const;

    /**
     * @brief Returns the sender of the message.
     *
     * @return The sender of the message.
     */
    [[nodiscard]] std::shared_ptr<Participant> getSender() const;

    /**
     * @brief Returns the timestamp of the message as a std::chrono::time_point.
     *
     * @return The timestamp of the message.
     */
    [[nodiscard]] std::chrono::time_point<std::chrono::system_clock> getTimestamp() const;

    /**
     * @brief Returns the timestamp of the message as a QDateTime object.
     *
     * @return The timestamp of the message.
     */
    [[nodiscard]] QDateTime getQTimestamp() const;

    /**
     * @brief Returns the name of the sender of the message.
     *
     * @return A string representation of the name of the sender of the message.
     */
    [[nodiscard]] QString getSenderString() const;

private:
    QString content;                           ///< The content of the message.
    const std::shared_ptr<Participant> sender; ///< The participant who sent the message.
    QDateTime sentDateTime;                    ///< The timestamp when the message was sent.
};

#endif
