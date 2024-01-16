#include "include/chat.h"

void Chat::add_message(Message msg) {
    messages.emplace(msg);
}


void Chat::foreach_do(std::function<void( const Message&)> func) {
    std::for_each(messages.begin(), messages.end(), func);
}
