#ifndef CLIENT_ORGANIZER_H
#define CLIENT_ORGANIZER_H

#include <unordered_map>

#include <QDateTime>
#include <stdlib.h>

// typedef unsigned client_key;

// // TODO make this thread safe
// // TODO don't allow client with same name

// class ClientInfo {
//         QString name;
//         QDateTime registered_since;
//         public:
//         ClientInfo(QString &name, QDateTime &registered_since) : name(name), registered_since(registered_since) {};
// };

// class ClientOrganizer {
//     std::unordered_map<client_key, ClientInfo> clients;
//     client_key get_new_rand_key() {
//         auto new_rand_int = std::rand();
//         unsigned new_uint_key = (new_rand_int / (RAND_MAX + 1u)) * UINT_MAX;
//         if (clients.find(new_uint_key) != clients.end()) {
//             // this may start an infinite recursion if there are UINT_MAX clients and all the keys are reserverd, but YAGNI
//             return get_new_rand_key();
//         }
//         else {
//             return new_uint_key;
//         }
//     }
//     public:
//     void addClient(QString &name, QDateTime &registered_since) {
//         clients.emplace(get_new_rand_key(), ClientInfo{name, registered_since});
//     }
// };

#endif