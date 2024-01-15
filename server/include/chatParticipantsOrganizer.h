#ifndef CHAT_ORGANIZER_H
#define CHAT_ORGANIZER_H

#include <unordered_map>
#include <map>
#include <optional>
#include <functional>

#include "chat.h"
#include "participant.h"

class ChatParticipantOrganizer {

    std::unique_ptr<Chat> global;
    std::unordered_map<participant_key, std::unordered_map<chat_key, std::shared_ptr<Chat>>> chats;
    public:
    void add_participant(participant_key participant_key_);
    void remove_participant(participant_key participant_key_);
    void add_new_chat(std::vector<participant_key> participants);
    auto get_chat_keys(participant_key participant_key_);
    std::optional<std::reference_wrapper<Chat>> get_chat(participant_key participant_key_, chat_key chat_key_);
};

#endif