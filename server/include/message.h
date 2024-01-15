#include <string>
#include <chrono>
#include <compare>

class Message {
    std::chrono::time_point<std::chrono::system_clock> date;
    std::string content;
    public:
    Message(std::string content) : content(content), date(std::chrono::system_clock::now()) {};
    auto operator<=>(const Message &msg) const = default;
};