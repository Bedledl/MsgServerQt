#include <string>
#include <set>
#include <QReadWriteLock>
#include <functional>

#include "message.h"
#include "clientOrganizer.h"


class Chat {
    const UniqueKey<Chat> key;
    std::set<Message> messages = {};
    QReadWriteLock lock;
    public:
    void foreach_do(std::function<void(const Message&)> func);
    void add_message(Message msg);
    const UniqueKey<Chat> &get_key() const { return key; };
};
