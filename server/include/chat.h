#include <string>
#include <set>
#include <mutex>

#include "message.h"
#include "clientOrganizer.h"

typedef unsigned chat_key;

class Chat {
    chat_key key;
    std::set<Message> messages = {};
    std::mutex mutex;
    public:
    Chat() {};
    std::set<Message> get_n_messages_from_offset(size_t offset, size_t n);
    void add_message(Message msg);
    chat_key get_key() const { return key; };
};
