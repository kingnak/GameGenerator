#ifndef GGVIEWPAGE_H
#define GGVIEWPAGE_H

#include <QRect>

class GGPage;
class GGViewModel;

class GGViewPage
{
public:
    GGViewPage(GGPage *page, GGViewModel *model, QRect bounds);

    GGPage *page() { return m_page; }
    QRect bounds() { return m_bounds; }
    void setBounds(QRect r);

private:
    GGPage *m_page;
    GGViewModel *m_model;
    QRect m_bounds;
};

#endif // GGVIEWPAGE_H
