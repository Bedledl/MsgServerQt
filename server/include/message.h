#include <string>
#include <chrono>
#include <compare>

class Message
{
    std::chrono::time_point<std::chrono::system_clock> date;
    QString content;

public:
    Message(QString content) : date(std::chrono::system_clock::now()), content(content){};
    std::strong_ordering operator<=>(const Message &msg) const
    {
        if (date == msg.date)
        {
            auto string_compare = QString::compare(content, msg.content, Qt::CaseSensitive);
            if (string_compare == 0)
            {
                return std::strong_ordering::equal;
            }
            else if (string_compare < 0)
            {
                return std::strong_ordering::less;
            }
            return std::strong_ordering::greater;
        }
        return date <=> msg.date;
    };
};