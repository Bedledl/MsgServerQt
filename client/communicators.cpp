#include <chrono>
#include <google/protobuf/util/time_util.h>

#include <QString>

#include <string>

#include "client.h"
#include "clientMsgFormats.pb.h"
#include "communicators.h"
#include "serverMsgFormats.pb.h"

std::string ClientCommunicator::answerMessage(std::string msg)
{
    ServerCommand incomingCmd;
    incomingCmd.ParseFromString(msg);
    switch (incomingCmd.cmd())
    {
    case ServerCommandId::ServerGenericResponse:
    {
        // TODO
        ;
        break;
    }
    case ServerCommandId::ServerChatCommand:
    {
        auto chatCmd = incomingCmd.chatcmd();
        switch (chatCmd.cmd())
        {
        case ServerChatCommandId::AddedToChat:
        {
            try
            {
                client.addNewChat(chatCmd.chatkey());
            }
            catch (ChatAlreadyExists &e)
            {
                qDebug() << e.what();
                return generateGenericResponseString(ResponseCode::ERROR);
            }
            return generateGenericResponseString(ResponseCode::SUCCESS);
            break;
        }
        case ServerChatCommandId::LeftChat:
        {
            try
            {
                client.leaveChat(chatCmd.chatkey());
            }
            catch (ChatNotFound &e)
            {
                qDebug() << e.what();
                return generateGenericResponseString(ResponseCode::ERROR);
            }
            return generateGenericResponseString(ResponseCode::SUCCESS);
            break;
        }
        default:
            return generateGenericResponseString(ResponseCode::MALFORMED_MESSAGE);
        }
        break;
    }
    case ServerCommandId::ServerParticipantCommand:
    {
        auto serverParticipantCommand = incomingCmd.participantcmd();
        auto key = serverParticipantCommand.participantkey();
        switch (serverParticipantCommand.cmd())
        {
        case ServerParticipantCommandId::SendEntryDate:
        {

            auto entryDate = serverParticipantCommand.timestamp();

            std::chrono::milliseconds ms = std::chrono::milliseconds(google::protobuf::util::TimeUtil::TimestampToMilliseconds(entryDate));
            QDateTime datetime;
            client.assignParticipantEntryDate(key, datetime.addDuration(ms));
            break;
        }
        case ServerParticipantCommandId::SendName:
        {
            auto name = serverParticipantCommand.name();
            client.assignParticipantName(key, QString::fromStdString(name));
            break;
        }
        default:
        {
            return generateGenericResponseString(ResponseCode::MALFORMED_MESSAGE);
        }
        }
        break;
    }
    case ServerCommandId::ServerMessageCommand:
    {
        auto msgCmd = incomingCmd.msgcmd();
        auto participantKey = msgCmd.participantkey();
        auto chatKey = msgCmd.chatkey();
        auto timestamp = msgCmd.timestamp();
        auto content = msgCmd.content();

        QDateTime datetime;
        datetime.setMSecsSinceEpoch(google::protobuf::util::TimeUtil::TimestampToMilliseconds(timestamp));

        try
        {
            client.addNewIncomingMessage(chatKey, QString::fromStdString(content), participantKey, datetime);
        }
        catch (ChatNotFound &e)
        {
            return generateGenericResponseString(ResponseCode::ERROR);
        }
        catch (ParticipantNotFound &e)
        {
            return generateGenericResponseString(ResponseCode::ERROR);
        }
        return generateGenericResponseString(ResponseCode::SUCCESS);
        break;
    }
    default:
    {
        return generateGenericResponseString(ResponseCode::MALFORMED_MESSAGE);
    }
    }
    return generateGenericResponseString(ResponseCode::MALFORMED_MESSAGE);
};

QString ClientCommunicator::answerMessage(QString msg)
{
    return QString::fromStdString(answerMessage(msg.toStdString()));
}

QString ClientCommunicator::welcomeMessage()
{
    ClientCommand ccc;
    ccc.set_cmd(ClientCommandId::SetNickname);
    auto nickname = client.getNickname();
    ccc.set_name(nickname.toStdString());
    return QString::fromStdString(ccc.SerializeAsString());
}
