#include <cstdint>

namespace Communication
{
    enum Command : uint8_t
    {
        setName,
        newChat,
        getChatKeys,
        GetParticipantKeys,
        NewMessage,
        leaveChat,
        getName,
        getEntryDate,
        addToChat
    };

    struct PromptHeader
    {
        Command command;
        uint8_t legnth;
    };

}
