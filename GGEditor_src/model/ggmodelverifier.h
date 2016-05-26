#ifndef GGMODELVERIFIER_H
#define GGMODELVERIFIER_H

#include <QList>
#include <QSet>
#include <model/ggconnectionslotdata.h>

class GGPage;
class GGConnection;
class GGAbstractModel;
class GGAbstractMediaResolver;

class GGModelError
{
public:
    enum Severity {
        None, Notice, Warning, Error, Fatal
    };
    GGModelError() : m_severity(None), m_page(NULL), m_connection(NULL) {}
    GGModelError(Severity level, const QString &message, const GGPage *page = NULL, const GGConnection *conn = NULL, GGConnectionSlotData slot = GGConnectionSlotData::NoConnection)
        : m_severity(level), m_message(message), m_page(page), m_connection(conn), m_slot(slot) {}

    Severity severity() const { return m_severity; }
    QString message() const { return m_message; }
    const GGPage *page() const { return m_page; }
    const GGConnection *connection() const { return m_connection; }
    GGConnectionSlotData slot() const { return m_slot; }

private:
    Severity m_severity;
    QString m_message;
    const GGPage *m_page;
    const GGConnection *m_connection;
    GGConnectionSlotData m_slot;
};

class GGModelErrorList : public QList<GGModelError>
{
public:
    GGModelErrorList() {}
    GGModelErrorList(const GGModelError &error) { append(error); }

    GGModelErrorList filtered(GGModelError::Severity level, bool andHigher = true) const;
};

class GGModelVerifier
{
public:
    GGModelVerifier();
    virtual ~GGModelVerifier();

    virtual GGModelErrorList verify(const GGAbstractModel *model);

protected:
    virtual void clear();
    const GGAbstractModel *model();

    GGModelErrorList checkPageName(const GGPage *page);
    GGModelErrorList checkStartPage(const GGPage *page);
    GGModelErrorList checkEndPage(const GGPage *page);
    GGModelErrorList checkDeadEnd(const GGPage *page);
    GGModelErrorList checkLooseConnection(const GGPage *page);
    GGModelErrorList checkVariables(const GGPage *page, const QStringList &validVariables);
    GGModelErrorList checkCondition(const GGPage *page);
    GGModelErrorList checkLinkTitles(const GGPage *page);
    GGModelErrorList checkContent(const GGPage *page);

    GGModelErrorList postCheckStartPage();
    GGModelErrorList postCheckEndPage();
    GGModelErrorList postCheckUnreachable();

    static const GGModelErrorList NoError;

private:
    void recursiveCheckUnreachable(const GGPage *page, QSet<const GGPage *> &visited);

private:
    const GGAbstractModel *m_model;
    QSet<QString> m_uniquePageNames;
    QList<const GGPage *> m_startPages;
    bool m_hasEndPage;
};

#endif // GGMODELVERIFIER_H
