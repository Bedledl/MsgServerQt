#include <string>
#include <chrono>
#include <compare>

class Message {
    std::chrono::time_point<std::chrono::system_clock> date;
    QString content;
    public:
    Message(QString content) : date(std::chrono::system_clock::now()), content(content) {};
    auto operator<=>(const Message &msg) const { return date <=> msg.date; };
};