#include <string>
#include <set>
#include <mutex>
#include <functional>

#include "message.h"
#include "clientOrganizer.h"

typedef unsigned chat_key;

class Chat {
    chat_key key;
    std::set<Message> messages = {};
    std::mutex mutex;
    public:
    void foreach_do(std::function<void(const Message&)> func);
    void add_message(Message msg);
    chat_key get_key() const { return key; };
};
