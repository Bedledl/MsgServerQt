#ifndef F01B6FBD_7838_45ED_B1E9_E721550C395D
#define F01B6FBD_7838_45ED_B1E9_E721550C395D
#include "chat.h"
#include <QAbstractListModel>

class ChatViewModel : public Chat, public virtual QAbstractListModel
{
    Q_OBJECT
public:
    ChatViewModel(ChatKey key, QObject *parent = nullptr) : Chat(key, parent), QAbstractListModel(parent) {};
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif /* F01B6FBD_7838_45ED_B1E9_E721550C395D */
