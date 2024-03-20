#ifndef SERVER_INCLUDE_RAWCLIENTMESSAGEPROCESSOR
#define SERVER_INCLUDE_RAWCLIENTMESSAGEPROCESSOR
#include <string>

#include "communicator.h"
#include "participant.h"
#include "server.h"
#include "uniqueKeysTypedef.h"

class ClientCommand_ClientChatCommand;
class ClientCommand_ClientParticipantCommand;
class ClientCommand_ClientMessageCommand;
class ClientCommand_ClientParticipantKeyListCommand;
class ClientCommand;
class ServerCommand;

class RawClientMessageProcessor : Communicator
{
public:
    RawClientMessageProcessor(ServerIface &server, ServerParticipant &participant, std::function<void(std::string)> sendFunc);
    void processRawMessage(std::string rawMessage);

private:
    ServerIface &server;
    ServerParticipant &participant;
    void processChatCommand(ServerCommand &outgoingCmd, const SequenceNumber seqNr, const ClientCommand_ClientChatCommand &clientChatCommand);
    void processParticipantCommand(ServerCommand &outgoingCmd, const SequenceNumber seqNr, const ClientCommand_ClientParticipantCommand &clientParticipantCommand);
    void processMessageCommand(ServerCommand &outgoingCmd, const SequenceNumber seqNr, const ClientCommand_ClientMessageCommand &clientMessageCommand);
    void processParticipantKeyListCommand(ServerCommand &outgoingCmd, const SequenceNumber seqNr, const ClientCommand_ClientParticipantKeyListCommand &clientParticipantKeyListCommand);
    void processGenericResponse(ServerCommand &outgoingCmd, const ClientCommand &clientGenericResponse);
};

#endif /* SERVER_INCLUDE_RAWCLIENTMESSAGEPROCESSOR */
