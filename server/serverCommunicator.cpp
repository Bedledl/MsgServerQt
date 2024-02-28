#include "clientMsgFormats.pb.h"
#include "clientThreadIface.h"
#include "communicators.h"
#include "participant.h"
#include "serverMsgFormats.pb.h"
#include <QString>

void ServerCommunicator::buildGenericResponse(ServerCommand &cmdBuf, ResponseCode code)
{
    cmdBuf.set_cmd(ServerCommandId::ServerGenericResponse);
    cmdBuf.set_response(code);
}

std::string ServerCommunicator::answerMessage(std::string msg)
{
    ClientCommand incomingCmd;
    incomingCmd.ParseFromString(msg);

    ServerCommand outgoingCmd;

    std::cout << "Incoming Client Message: ";

    switch (incomingCmd.cmd())
    {

    case ClientCommandId::SetNickname:
    {
        auto name = incomingCmd.msgcmd().content();
        clientThreadWorker.setNickname(name);
        buildGenericResponse(outgoingCmd, ResponseCode::SUCCESS);
        std::cout << "Client Set Nickname Cmd\n";
        break;
    }

    case ClientCommandId::NewChat:
    {
        auto newChatKey = clientThreadWorker.requestNewChat();

        outgoingCmd.set_cmd(ServerCommandId::ServerChatCommand);
        auto chatCmd = new ServerCommand_ServerChatCommand();

        chatCmd->set_cmd(ServerChatCommandId::AddedToChat);
        chatCmd->set_chatkey(newChatKey);

        outgoingCmd.set_allocated_chatcmd(chatCmd);

        std::cout << "Client New Chat Cmd\n";
        break;
    }

    case ClientCommandId::ClientGenericResponse:
    {
        auto inResponseCode = incomingCmd.response();
        std::cout << "Client Generic Response: ";
        switch (inResponseCode)
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
    break;

    case ClientCommandId::ClientChatCommand:
    {
        auto chatCmd = incomingCmd.chatcmd();
        auto chatKey = chatCmd.chatkey();

        // TODO int to unsigned
        // ptofobuf usigned?
        switch (chatCmd.cmd())
        {

        case ClientChatCommandId::GetParticipantKeys:
        {
            std::cout << "Get Participant Keys\n";
            try
            {
                clientThreadWorker.requestChatParticipantKeys(chatKey);
            }
            catch (const std::exception &e)
            {
                buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
                break;
            }
            buildGenericResponse(outgoingCmd, ResponseCode::SUCCESS);
            break;
        }

        case ClientChatCommandId::LeaveChat:
        {
            std::cout << "Leave Chat\n";
            try
            {
                clientThreadWorker.requestLeavingChat(chatKey);
            }
            catch (const ChatNotFound &e)
            {
                buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
                break;
            }
            buildGenericResponse(outgoingCmd, ResponseCode::SUCCESS);
            break;
        }

        default:
        {
            std::cout << "Unknown Message\n";
            buildGenericResponse(outgoingCmd, ResponseCode::MALFORMED_MESSAGE);
        }
        }
    }
    break;

    case ClientCommandId::ClientParticipantCommand:
    {
        auto partCmd = incomingCmd.participantcmd();
        auto participantKey = partCmd.participantkey();
        switch (partCmd.cmd())
        {

        case ClientParticipantCommandId::GetName:
        {
            std::cout << "Get Participant Name\n";

            auto participantCmd = new ServerCommand_ServerParticipantCommand();
            participantCmd->set_cmd(ServerParticipantCommandId::SendName);
            participantCmd->set_participantkey(participantKey);

            try
            {
                auto name = clientThreadWorker.getServer().getParticipantName(participantKey);
                participantCmd->set_name(name);
            }
            catch (const ParticipantNotFound &e)
            {
                qDebug() << e.what();
                buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
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
                auto qTs = clientThreadWorker.getServer().getParticipantEntryDate(participantKey);
                auto ts = new google::protobuf::Timestamp();
                ts->set_seconds(qTs.toSecsSinceEpoch());
                participantCmd->set_allocated_timestamp(ts);
            }
            catch (const ParticipantNotFound &e)
            {
                buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
                break;
            }

            outgoingCmd.set_cmd(ServerCommandId::ServerParticipantCommand);
            outgoingCmd.set_allocated_participantcmd(participantCmd);
            break;
        }

        case ClientParticipantCommandId::AddToChat:
        {
            std::cout << "Add Participant To Chat\n";
            auto chatKey = partCmd.chatkey();

            try
            {
                auto chatPtr = clientThreadWorker.getParticipant().getChat(chatKey);
                try
                {
                    clientThreadWorker.getServer().addParticipantToChat(chatPtr, participantKey);
                    buildGenericResponse(outgoingCmd, ResponseCode::SUCCESS);
                }
                catch (const ParticipantNotFound &e)
                {
                    buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
                }
                catch (const ParticipantAlreadyExists &e)
                {
                    buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
                }
            }
            catch (const ChatNotFound &e)
            {
                buildGenericResponse(outgoingCmd, ResponseCode::ERROR);
                break;
            }
            break;
        }
        break;

        default:
        {
            std::cout << "Unknown Message\n";
            buildGenericResponse(outgoingCmd, ResponseCode::MALFORMED_MESSAGE);
        }
        }
        break;
    }

    default:
    {
        std::cout << "Unknown Message\n";
        buildGenericResponse(outgoingCmd, ResponseCode::MALFORMED_MESSAGE);
    }
    }
    return outgoingCmd.SerializeAsString();
}

QString ServerCommunicator::welcomeMessage()
{
    ServerCommand addedToGlobalChat;
    addedToGlobalChat.set_cmd(ServerCommandId::ServerChatCommand);

    auto *internalAddedToGlobalChat = new ServerCommand_ServerChatCommand();
    auto globalChat = clientThreadWorker.getServer().getGlobalChat();
    internalAddedToGlobalChat->set_chatkey(int32_t(globalChat->getKey()));
    internalAddedToGlobalChat->set_cmd(ServerChatCommandId::AddedToChat);
    addedToGlobalChat.set_allocated_chatcmd(internalAddedToGlobalChat);

    return QString::fromStdString(addedToGlobalChat.SerializeAsString());
}
