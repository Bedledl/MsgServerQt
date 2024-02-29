#include "messageModelItem.h"

#include "moc_messageModelItem.cpp"

QString MessageModelItem::getContent() const
{
    return message->getContent();
}
QDateTime MessageModelItem::getTimestamp() const
{
    return message->getQTimestamp();
}
QString MessageModelItem::getSender() const
{
    return message->getSenderString();
}