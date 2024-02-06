#include "include/appGui.h"
#include "chat.h"
#include "chatGui.h"
#include "moc_appGui.cpp"
#include <iostream>

void AppGui::add_chat(ChatGui *newChat)
{
    chats.append(newChat);
}
