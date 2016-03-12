#ifndef GGMOUSELABEL_H
#define GGMOUSELABEL_H

#include <QLabel>

class GGMouseLabel : public QLabel
{
    Q_OBJECT
public:
    GGMouseLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);
    GGMouseLabel(const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0);

signals:
    void hoverEnter();
    void hoverLeave();

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
};

#endif // GGMOUSELABEL_H
