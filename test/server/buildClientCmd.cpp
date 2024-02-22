#include <buildClientCmd.h>
#include <clientMsgFormats.pb.h>
#include <string>

TestClientCommunicator::TestClientCommunicator(ChatKey chatKey, ParticipantKey participantKey, std::string participantName, std::string testMessageContent, QDateTime timestamp)
    : chatKey(chatKey), participantKey(participantKey), participantName(participantName), testMessageContent(testMessageContent), timestamp(timestamp) {}

std::string TestClientCommunicator::setNickname()
{
    ClientCommand clientCmd;
    clientCmd.set_cmd(ClientCommandId::SetNickname);
    clientCmd.set_name(participantName);
    return clientCmd.SerializeAsString();
}
std::string TestClientCommunicator::requestNewChat()
{
    ClientCommand clientCmd;
    clientCmd.set_cmd(ClientCommandId::NewChat);

    return clientCmd.SerializeAsString();
}
std::string TestClientCommunicator::requestChatParticipantKeys()
{
    ClientCommand clientCmd;
    clientCmd.set_cmd(ClientCommandId::ClientChatCommand);

    auto clientChatCmd = new ClientCommand_ClientChatCommand();
    clientChatCmd->set_cmd(ClientChatCommandId::GetParticipantKeys);
    clientChatCmd->set_chatkey(chatKey);

    clientCmd.set_allocated_chatcmd(clientChatCmd);
    return clientCmd.SerializeAsString();
}
std::string TestClientCommunicator::requestLeavingChat()
{
    ClientCommand clientCmd;
    clientCmd.set_cmd(ClientCommandId::ClientChatCommand);

    auto clientChatCmd = new ClientCommand_ClientChatCommand();
    clientChatCmd->set_cmd(ClientChatCommandId::LeaveChat);
    clientChatCmd->set_chatkey(chatKey);

    clientCmd.set_allocated_chatcmd(clientChatCmd);
    return clientCmd.SerializeAsString();
}
std::string TestClientCommunicator::requestParticipantName()
{
    ClientCommand clientCmd;
    clientCmd.set_cmd(ClientCommandId::ClientParticipantCommand);

    auto clientParticipantCmd = new ClientCommand_ClientParticipantCommand();
    clientParticipantCmd->set_cmd(ClientParticipantCommandId::GetName);
    clientParticipantCmd->set_participantkey(participantKey);

    clientCmd.set_allocated_participantcmd(clientParticipantCmd);

    return clientCmd.SerializeAsString();
}
std::string TestClientCommunicator::requestParticipantEntryDate()
{
    ClientCommand clientCmd;
    clientCmd.set_cmd(ClientCommandId::ClientParticipantCommand);

    auto clientParticipantCmd = new ClientCommand_ClientParticipantCommand();
    clientParticipantCmd->set_cmd(ClientParticipantCommandId::GetEntryDate);
    clientParticipantCmd->set_participantkey(participantKey);

    clientCmd.set_allocated_participantcmd(clientParticipantCmd);
    return clientCmd.SerializeAsString();
}
std::string TestClientCommunicator::requestAddingParticipantToChat()
{
    ClientCommand clientCmd;
    clientCmd.set_cmd(ClientCommandId::ClientParticipantCommand);

    auto clientParticipantCmd = new ClientCommand_ClientParticipantCommand();
    clientParticipantCmd->set_cmd(ClientParticipantCommandId::AddToChat);
    clientParticipantCmd->set_participantkey(participantKey);

    clientCmd.set_allocated_participantcmd(clientParticipantCmd);
    return clientCmd.SerializeAsString();
}