#ifndef CLIENT_ORGANIZER_H
#define CLIENT_ORGANIZER_H

#include <stdlib.h>
#include <unordered_map>

#include "participant.h"

// TODO make this thread safe
// TODO don't allow client with same name

class ClientOrganizer
{
    std::unordered_map<unsigned, Participant *> clients;

public:
    void registerClient(Participant &new_participant)
    {
        clients.emplace(new_participant.getKey(), &new_participant);
    };
    void unregisterClient(unsigned key)
    {
        clients.erase(key);
    };
};

#endif