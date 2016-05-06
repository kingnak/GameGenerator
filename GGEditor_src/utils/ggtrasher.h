#ifndef GGTRASHER_H
#define GGTRASHER_H

#include <QDir>

class GGTrasher
{
public:
    GGTrasher() {}
    virtual ~GGTrasher() {}

    static GGTrasher *trasher();
    static void setTrasher(GGTrasher *trasher);

    virtual bool moveToTrash(const QString &file) = 0;


private:
    static GGTrasher *s_trasher;
};

class GGDefaultTrasher : public GGTrasher
{
public:
    GGDefaultTrasher();

    static QString TRASH_DIR_NAME;

    void setBaseDir(QDir dir);
    QDir baseDir() const;
    virtual bool moveToTrash(const QString &file);

private:
    QDir m_baseDir;
};

#endif // GGTRASHER_H
