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
            try{
                client.addNewChat(chatCmd.chatkey());
            }
            catch (ChatAlreadyExists &e){
                qDebug() << e.what();
                return generateGenericResponseString(ResponseCode::ERROR);
            }
            break;
        }
        case ServerChatCommandId::LeftChat:
        {
            client.leaveChat(chatCmd.chatkey());
            break;
        }
        case ServerChatCommandId::ServerNewMessage:
        {
            auto data = chatCmd.data().chatmsg();
            auto participantKey = data.originator_key();
            auto timestamp = data.timestamp();
            auto content = data.content();

            std::chrono::milliseconds ms = std::chrono::milliseconds(google::protobuf::util::TimeUtil::TimestampToMilliseconds(timestamp));
            QDateTime datetime;
            client.addNewIncomingMessage(chatCmd.chatkey(), QString(content.c_str()), participantKey, datetime.addDuration(ms));
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
            auto entryDate = serverParticipantCommand.data().date();

            std::chrono::milliseconds ms = std::chrono::milliseconds(google::protobuf::util::TimeUtil::TimestampToMilliseconds(entryDate));
            QDateTime datetime;
            client.assignParticipantEntryDate(key, datetime.addDuration(ms));
            break;
        }
        case ServerParticipantCommandId::SendName:
        {
            auto name = serverParticipantCommand.data().name().name();
            client.assignParticipantName(key, QString(name.c_str()));
            break;
        }
        default:
        {
            return generateGenericResponseString(ResponseCode::MALFORMED_MESSAGE);
        }
        }
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

    auto name = new Name();
    name->set_name("Erika Musterfrau"); //TODO
    name->set_namelength(17);

    auto data = new Data();
    data->set_allocated_name(name);
    ccc.set_allocated_data(data);
    return QString(ccc.SerializeAsString().c_str());
}
