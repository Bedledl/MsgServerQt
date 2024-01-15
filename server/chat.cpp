#include "include/chat.h"

void Chat::add_message(Message msg) {
    messages.emplace(msg);
}


