#include <string>

#include "proto/clientMsgFormats.pb.h"
#include "proto/serverMsgFormats.pb.h"
#include "uniqueKeysTypedef.h"

#include "rawClientMessageProcessor.h"

RawClientMessageProcessor::RawClientMessageProcessor(ServerIface &server, ServerParticipant &participant, std::function<void(std::string)> sendFunc) : Communicator(sendFunc), server(server), participant(participant) {}

void RawClientMessageProcessor::processChatCommand(ServerCommand &outgoingCmd, const SequenceNumber seqNr, const ClientCommand_ClientChatCommand &clientChatCommand)
{
    auto chatKey = clientChatCommand.chatkey();
    switch (clientChatCommand.cmd())
    {

    case ClientChatCommandId::GetParticipantKeys:
    {
        std::cout << "Get Participant Keys\n";
        try
        {
            auto keys = participant.getParticipantKeys(chatKey);

            auto partKeysList = new ServerCommand_ServerParticipantKeyListCommand();
            partKeysList->set_chatkey(chatKey);
            for (auto key : keys)
            {
                partKeysList->add_participantkeys(key);
            }
            outgoingCmd.set_cmd(ServerCommandId::ServerParticipantKeyListCommand);
            outgoingCmd.set_allocated_participantkeylist(partKeysList);
        }
        catch (const ChatNotFound &e)
        {
            //// buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
            break;
        }
        //// buildGenericResponse(outgoingCmd, ResponseCode::SUCCESS);
        break;
    }

    case ClientChatCommandId::LeaveChat:
    {
        std::cout << "Leave Chat\n";
        try
        {
            participant.leaveChat(chatKey);
        }
        catch (const ChatNotFound &e)
        {
            // buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
            break;
        }
        // buildGenericResponse(outgoingCmd, ResponseCode::SUCCESS);
        break;
    }

    default:
    {
        std::cout << "Unknown Message\n";
        // buildGenericResponse(outgoingCmd, ResponseCode::MALFORMED_MESSAGE);
    }
    }
}
void RawClientMessageProcessor::processParticipantCommand(ServerCommand &outgoingCmd, const SequenceNumber seqNr, const ClientCommand_ClientParticipantCommand &clientParticipantCommand)
{
    auto participantKey = clientParticipantCommand.participantkey();
    switch (clientParticipantCommand.cmd())
    {

    case ClientParticipantCommandId::GetName:
    {
        std::cout << "Get Participant Name\n";

        auto participantCmd = new ServerCommand_ServerParticipantCommand();
        participantCmd->set_cmd(ServerParticipantCommandId::SendName);
        participantCmd->set_participantkey(participantKey);

        try
        {
            auto name = server.getParticipantName(participantKey);
            participantCmd->set_name(name);
        }
        catch (const ParticipantNotFound &e)
        {
            qDebug() << e.what();
            // buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
            break;
        }

        outgoingCmd.set_cmd(ServerCommandId::ServerParticipantCommand);
        outgoingCmd.set_allocated_participantcmd(participantCmd);
        break;
    }

    case ClientParticipantCommandId::GetEntryDate:
    {
        std::cout << "Get Participant Entry Date\n";
        auto participantCmd = new ServerCommand_ServerParticipantCommand();
        participantCmd->set_cmd(ServerParticipantCommandId::SendName);
        participantCmd->set_participantkey(participantKey);

        try
        {
            auto qTs = server.getParticipantEntryDate(participantKey);
            auto ts = new google::protobuf::Timestamp();
            ts->set_seconds(qTs.toSecsSinceEpoch());
            participantCmd->set_allocated_timestamp(ts);
        }
        catch (const ParticipantNotFound &e)
        {
            // buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
            break;
        }

        outgoingCmd.set_cmd(ServerCommandId::ServerParticipantCommand);
        outgoingCmd.set_allocated_participantcmd(participantCmd);
        break;
    }

    case ClientParticipantCommandId::AddToChat:
    {
        std::cout << "Add Participant To Chat\n";
        auto chatKey = clientParticipantCommand.chatkey();

        try
        {
            auto chatPtr = participant.getChat(chatKey);
            try
            {
                server.addParticipantToChat(chatPtr, participantKey);
                // buildGenericResponse(outgoingCmd, ResponseCode::SUCCESS);
            }
            catch (const ParticipantNotFound &e)
            {
                // buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
            }
            catch (const ParticipantAlreadyExists &e)
            {
                // buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
            }
        }
        catch (const ChatNotFound &e)
        {
            // buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
            break;
        }
        break;
    }
    break;

    default:
    {
        std::cout << "Unknown Message\n";
        // buildGenericResponse(outgoingCmd, ResponseCode::MALFORMED_MESSAGE);
    }
    }
}

void RawClientMessageProcessor::processMessageCommand(ServerCommand &outgoingCmd, const SequenceNumber seqNr, const ClientCommand_ClientMessageCommand &clientMessageCommand)
{
    auto chatKey = clientMessageCommand.chatkey();
    auto content = clientMessageCommand.content();
    auto datetime = clientMessageCommand.timestamp();
    participant.newMessage(chatKey, content, QDateTime::fromSecsSinceEpoch(datetime.seconds()));
}

void RawClientMessageProcessor::processGenericResponse(ServerCommand &outgoingCmd, const ClientCommand &clientGenericResponse)
{
    std::cout << "Client Generic Response: ";
    switch (clientGenericResponse.response())
    {
    case ResponseCode::SUCCESS:
        std::cout << "Success\n";
        break;

    case ResponseCode::ERROR:
        std::cout << "Error\n";
        break;

    case ResponseCode::MALFORMED_MESSAGE:
        std::cout << "Malformed\n";
        break;

    default:
        std::cout << "Unknown Response Code\n";
    }
    // TODO
}

void RawClientMessageProcessor::processRawMessage(std::string rawMessage)
{
    ClientCommand incomingCmd;
    incomingCmd.ParseFromString(rawMessage);

    ServerCommand outgoingCmd;

    switch (incomingCmd.cmd())
    {

    case ClientCommandId::SetNickname:
    {
        participant.setNickname(QString::fromStdString(incomingCmd.name()));

        // buildGenericResponse(outgoingCmd, ResponseCode::SUCCESS);
        break;
    }

    case ClientCommandId::NewChat:
    {
        auto newChatKey = participant.newChat();

        outgoingCmd.set_cmd(ServerCommandId::ServerChatCommand);
        auto clientChatCommand = outgoingCmd.mutable_chatcmd();

        clientChatCommand->set_cmd(ServerChatCommandId::AddedToChat);
        clientChatCommand->set_chatkey(newChatKey);

        std::cout << "Client New Chat Cmd\n";
        break;
    }

    case ClientCommandId::ClientGenericResponse:
    {
        processGenericResponse(outgoingCmd, incomingCmd);
        break;
    }

    case ClientCommandId::ClientChatCommand:
    {
        processChatCommand(outgoingCmd, incomingCmd.sequencenr(), incomingCmd.chatcmd());
        break;
    }

    case ClientCommandId::ClientParticipantCommand:
    {
        processParticipantCommand(outgoingCmd, incomingCmd.sequencenr(), incomingCmd.participantcmd());
        break;
    }

    case ClientCommandId::ClientMessageCommand:
    {
        processMessageCommand(outgoingCmd, incomingCmd.sequencenr(), incomingCmd.msgcmd());
        break;
    }

    default:
    {
        std::cout << "Unknown Message\n";
        // buildGenericResponse(outgoingCmd, ResponseCode::MALFORMED_MESSAGE);
    }
    }
    // TODO
    qDebug() << outgoingCmd.SerializeAsString();
}
