#include "include/appGui.h"
#include "moc_appGui.cpp"
#include <iostream>

void AppGui::add_chat(ChatGui *newChat)
{
    chats.append(newChat);
}
