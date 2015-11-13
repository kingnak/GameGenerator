#ifndef GGVIEWCONNECTION_H
#define GGVIEWCONNECTION_H

class GGConnection;
class GGViewModel;

class GGViewConnection
{
public:
    GGViewConnection(GGConnection *conn, GGViewModel *model);

    GGConnection *connection() { return m_conn; }

private:
    GGConnection *m_conn;
    GGViewModel *m_model;
};

#endif // GGVIEWCONNECTION_H
