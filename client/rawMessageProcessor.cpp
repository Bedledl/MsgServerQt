#include <QDateTime>
#include <google/protobuf/util/time_util.h>

#include "chat.h"
#include "clientIface.h"
#include "participant.h"
#include "proto/serverMsgFormats.pb.h"
#include "rawMessageProcessor.h"

RawMessageProcessor::RawMessageProcessor(ClientOuterIface &client, std::function<void(std::string)> sendFunc) : Communicator(sendFunc), client(client) {}

void RawMessageProcessor::processRawMessage(std::string rawMessage)
{
    ServerCommand incomingCmd;
    incomingCmd.ParseFromString(rawMessage);
    auto seqNr = incomingCmd.sequencenr();

    if (!incomingCmd.has_cmd())
    {
        client.receivedMalformedMessage(seqNr);
        return;
    }

    switch (incomingCmd.cmd())
    {
    case ServerCommandId::ServerGenericResponse:
    {
        processGenericResponse(incomingCmd);
        break;
    }
    case ServerCommandId::ServerChatCommand:
    {
        processChatCommand(seqNr, incomingCmd.chatcmd());
        break;
    }
    case ServerCommandId::ServerParticipantCommand:
    {
        processParticipantCommand(seqNr, incomingCmd.participantcmd());
        break;
    }
    case ServerCommandId::ServerMessageCommand:
    {
        processMessageCommand(seqNr, incomingCmd.msgcmd());
        break;
    }
    case ServerCommandId::ServerParticipantKeyListCommand:
    {
        processParticipantKeyListCommand(seqNr, incomingCmd.participantkeylist());
        break;
    }
    default:
    {
        client.receivedMalformedMessage(seqNr);
        return;
    }
    }
};

void RawMessageProcessor::processChatCommand(const SequenceNumber seqNr, const ServerCommand_ServerChatCommand &serverChatCommand)
{
    if (!serverChatCommand.has_cmd())
    {
        client.receivedMalformedMessage(seqNr);
        return;
    }

    switch (serverChatCommand.cmd())
    {
    case ServerChatCommandId::AddedToChat:
    {
        try
        {
            client.addNewChat(seqNr, serverChatCommand.chatkey());
        }
        catch (ChatAlreadyExists &e)
        {
            qDebug() << e.what();
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        // return generateGenericResponseString(ResponseCode::SUCCESS);
        break;
    }
    case ServerChatCommandId::LeftChat:
    {
        try
        {
            client.leaveChat(seqNr, serverChatCommand.chatkey());
        }
        catch (ChatNotFound &e)
        {
            qDebug() << e.what();
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        // return generateGenericResponseString(ResponseCode::SUCCESS);
        break;
    }
    case ServerChatCommandId::ParticipantAddedToChat:
    {
        auto participantKey = serverChatCommand.participantkey();
        auto chatKey = serverChatCommand.chatkey();
        try
        {
            client.addParticipantToChat(seqNr, chatKey, participantKey);
        }
        catch (ChatNotFound &e)
        {
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        catch (ParticipantNotFound &e)
        {
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        // return generateGenericResponseString(ResponseCode::SUCCESS);
        break;
    }
    case ServerChatCommandId::ParticipantLeftChat:
    {
        auto participantKey = serverChatCommand.participantkey();
        auto chatKey = serverChatCommand.chatkey();
        try
        {
            client.removeParticipantFromChat(seqNr, chatKey, participantKey);
        }
        catch (ChatNotFound &e)
        {
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        catch (ParticipantNotFound &e)
        {
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        // return generateGenericResponseString(ResponseCode::SUCCESS);
        break;
    }
    default:
        client.receivedMalformedMessage(seqNr);
        return;
    }
}
void RawMessageProcessor::processParticipantCommand(const SequenceNumber seqNr, const ServerCommand_ServerParticipantCommand &serverParticipantCommand)
{
    auto key = serverParticipantCommand.participantkey();

    if (!serverParticipantCommand.has_cmd())
    {
        client.receivedMalformedMessage(seqNr);
        return;
    }

    switch (serverParticipantCommand.cmd())
    {
    case ServerParticipantCommandId::Added:
    {
        try
        {
            client.addParticipant(seqNr, key);
        }
        catch (ParticipantAlreadyExists &e)
        {
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        // return generateGenericResponseString(ResponseCode::SUCCESS);
        break;
    }
    case ServerParticipantCommandId::Removed:
    {
        try
        {
            client.removeParticipant(seqNr, key);
        }
        catch (ParticipantNotFound &e)
        {
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        // return generateGenericResponseString(ResponseCode::SUCCESS);
        break;
    }
    case ServerParticipantCommandId::SendEntryDate:
    {
        const auto &entryDate = serverParticipantCommand.timestamp();
        QDateTime datetime;
        datetime.setMSecsSinceEpoch(google::protobuf::util::TimeUtil::TimestampToMilliseconds(entryDate));
        try
        {
            client.assignParticipantEntryDate(seqNr, key, datetime);
        }
        catch (ParticipantNotFound &e)
        {
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        // return generateGenericResponseString(ResponseCode::SUCCESS);
        break;
    }
    case ServerParticipantCommandId::SendName:
    {
        const auto &name = serverParticipantCommand.name();

        try
        {
            client.assignParticipantName(seqNr, key, QString::fromStdString(name));
        }
        catch (ParticipantNotFound &e)
        {
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
        // return generateGenericResponseString(ResponseCode::SUCCESS);
        break;
    }
    default:
    {
        client.receivedMalformedMessage(seqNr);
        return;
    }
    }
}
void RawMessageProcessor::processMessageCommand(const SequenceNumber seqNr, const ServerCommand_ServerMessageCommand &serverMessageCommand)
{
    if (!(serverMessageCommand.has_participantkey() && serverMessageCommand.has_chatkey() && serverMessageCommand.has_timestamp() && serverMessageCommand.has_content()))
    {
        client.receivedMalformedMessage(seqNr);
        return;
    }

    auto participantKey = serverMessageCommand.participantkey();
    auto chatKey = serverMessageCommand.chatkey();
    const auto &timestamp = serverMessageCommand.timestamp();
    const auto &content = serverMessageCommand.content();

    QDateTime datetime;
    datetime.setMSecsSinceEpoch(google::protobuf::util::TimeUtil::TimestampToMilliseconds(timestamp));

    try
    {
        client.addNewIncomingMessage(seqNr, chatKey, QString::fromStdString(content), participantKey, datetime);
    }
    catch (ChatNotFound &e)
    {
        // return generateGenericResponseString(ResponseCode::ERROR);
    }
    catch (ParticipantNotFound &e)
    {
        // return generateGenericResponseString(ResponseCode::ERROR);
    }
    // return generateGenericResponseString(ResponseCode::SUCCESS);
}
void RawMessageProcessor::processParticipantKeyListCommand(const SequenceNumber seqNr, const ServerCommand_ServerParticipantKeyListCommand &serverParticipantKeyListCommand)
{
    auto keys = serverParticipantKeyListCommand.participantkeys();
    auto chatKey = serverParticipantKeyListCommand.chatkey();

    for (auto &key : keys)
    {

        try
        {
            client.addParticipantToChat(seqNr, chatKey, key);
        }
        catch (ParticipantAlreadyExists &e)
        {
            continue;
        }
        catch (const ChatNotFound &e)
        {
            // return generateGenericResponseString(ResponseCode::ERROR);
        }
    }

    // return generateGenericResponseString(ResponseCode::SUCCESS);
}
void RawMessageProcessor::processGenericResponse(const ServerCommand &serverGenericResponse)
{
    if (!serverGenericResponse.has_response())
    {
        client.receivedMalformedMessage(serverGenericResponse.sequencenr());
    }

    switch (serverGenericResponse.response())
    {
    case ResponseCode::SUCCESS:
    {
        qDebug() << "Success";
        // TODO answer queue
        break;
    }
    case ResponseCode::ERROR:
    {
        qDebug() << "Error";
        // answer queue
        break;
    }
    case ResponseCode::MALFORMED_MESSAGE:
    {
        qDebug() << "Malformed message";

        break;
    }
    default:
    {
        client.receivedMalformedMessage(serverGenericResponse.sequencenr());
    }
    }
}
