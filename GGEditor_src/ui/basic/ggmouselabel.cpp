#include "ggmouselabel.h"

GGMouseLabel::GGMouseLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{

}

GGMouseLabel::GGMouseLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{

}

void GGMouseLabel::enterEvent(QEvent *e)
{
    QLabel::enterEvent(e);
    emit hoverEnter();
}

void GGMouseLabel::leaveEvent(QEvent *e)
{
    QLabel::leaveEvent(e);
    emit hoverLeave();
}
