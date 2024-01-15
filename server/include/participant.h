

typedef unsigned participant_key;

class Participant {
        QString name;
        QDateTime registered_since;
        public:
        Participant(QString &name, QDateTime &registered_since) : name(name), registered_since(registered_since) {};
};
