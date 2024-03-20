#ifndef CLIENT_INCLUDE_RAWMESSAGEPROCESSOR
#define CLIENT_INCLUDE_RAWMESSAGEPROCESSOR
#include "communicator.h"
#include "uniqueKeysTypedef.h"
#include <string>

class ServerCommand_ServerChatCommand;
class ServerCommand_ServerParticipantCommand;
class ServerCommand_ServerMessageCommand;
class ServerCommand_ServerParticipantKeyListCommand;
class ServerCommand;
class ClientOuterIface;

class RawMessageProcessor : Communicator
{
public:
    RawMessageProcessor(ClientOuterIface &client, std::function<void(std::string)> sendFunc);
    void processRawMessage(std::string rawMessage) override;

private:
    ClientOuterIface &client;

    void processChatCommand(SequenceNumber seqNr, const ServerCommand_ServerChatCommand &serverChatCommand);
    void processParticipantCommand(SequenceNumber seqNr, const ServerCommand_ServerParticipantCommand &serverParticipantCommand);
    void processMessageCommand(SequenceNumber seqNr, const ServerCommand_ServerMessageCommand &serverMessageCommand);
    void processParticipantKeyListCommand(SequenceNumber seqNr, const ServerCommand_ServerParticipantKeyListCommand &serverParticipantKeyListCommand);
    void processGenericResponse(const ServerCommand &serverGenericResponse);
};

#endif /* CLIENT_INCLUDE_RAWMESSAGEPROCESSOR */
