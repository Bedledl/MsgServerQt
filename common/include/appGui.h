#ifndef APP_GUI_H
#define APP_GUI_H

#include <QDialog>
#include <QVector>

class ChatGui;

// dialog that contains container of Chat guis
// may allow to set the chats in own windows

class AppGui : public QDialog
{
    Q_OBJECT

     QVector<ChatGui *> chats;

public:
    AppGui(QWidget *parent = nullptr) : QDialog(parent){};
    void add_chat(ChatGui *newChat);
};

#endif