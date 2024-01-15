#include "include/chatParticipantsOrganizer.h"

void ChatParticipantOrganizer::add_participant(participant_key participant_key_) {
    chats.emplace(participant_key_, std::unordered_map<chat_key, std::shared_ptr<Chat>>{});
}

void ChatParticipantOrganizer::remove_participant(participant_key participant_key_) {
    auto chats_to_participant = chats.find(participant_key_);
    if (chats_to_participant != chats.end()) {
        chats.erase(chats_to_participant);
    }
}

void ChatParticipantOrganizer::add_new_chat(std::vector<participant_key> participants) {
    auto new_chat = std::make_shared<Chat>();
    for (auto &participant_key_ : participants) {
        chats[participant_key_][new_chat->get_key()] = {new_chat};
    }
}

auto ChatParticipantOrganizer::get_chat_keys(participant_key participant_key_) {
    std::vector<participant_key> keys;
    for (auto &[chat_key, chat] : chats[participant_key_]) {
        keys.push_back(chat_key);
    }

    keys.push_back(global->get_key());
    return keys;
}

std::optional<std::reference_wrapper<Chat>> ChatParticipantOrganizer::get_chat(participant_key participant_key_, chat_key chat_key_) {
    if (chat_key_ == global->get_key()) {
        return std::reference_wrapper<Chat>(*global);
    }

    return std::reference_wrapper<Chat>(*chats[participant_key_][chat_key_]);
}
