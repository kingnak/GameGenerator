#include "ggselectionitem.h"
#include "ggpageitem.h"
#include <QtGui>
#include <QGraphicsSceneMouseEvent>

class GGSelectionItem::GGCornerGrabber : public QGraphicsItem {
    friend class GGSelectionItem;
    GGCornerGrabber(int fX, int fY, GGSelectionItem *sel) : QGraphicsItem(sel), m_fX(fX), m_fY(fY) {
        installSceneEventFilter(sel);
    }
    int m_fX, m_fY;
    static const qreal GrabberSize;
    QPointF m_dragStart;
public:
    QRectF boundingRect() const { return QRectF(-GrabberSize,-GrabberSize,GrabberSize,GrabberSize); }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        painter->fillRect(boundingRect(), Qt::white);
        painter->setPen(Qt::blue);
        painter->drawRect(boundingRect());
    }
};
const qreal GGSelectionItem::GGCornerGrabber::GrabberSize = 5;

GGSelectionItem::GGSelectionItem(QGraphicsItem *parent)
    : QGraphicsItem(parent),
      m_wrapped(NULL)
{
}

void GGSelectionItem::setWrappedItem(GGPageItem *item)
{
    m_wrapped = item;
    if (!m_wrapped) return;
    setPos(m_wrapped->pos());
    updateCornerGrabberPos();
    update();
}

QRectF GGSelectionItem::boundingRect() const
{
    const qreal d = GGCornerGrabber::GrabberSize/2;
    return wrappedRect().adjusted(-d,-d,d,d);
}

void GGSelectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::blue);
    painter->drawRect(wrappedRect());
}

void GGSelectionItem::init()
{
    m_grabbers << new GGCornerGrabber(-1,-1,this)
               << new GGCornerGrabber( 0,-1,this)
               << new GGCornerGrabber(+1,-1,this)
               << new GGCornerGrabber(-1, 0,this)
               << new GGCornerGrabber(+1, 0,this)
               << new GGCornerGrabber(-1,+1,this)
               << new GGCornerGrabber( 0,+1,this)
               << new GGCornerGrabber(+1,+1,this)
                  ;
}

bool GGSelectionItem::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    GGCornerGrabber *corner = dynamic_cast<GGCornerGrabber*>(watched);
    if (!corner) return false;

    QGraphicsSceneMouseEvent * mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
    if (!mevent) return false;

    if (mevent->buttons() != Qt::LeftButton) return false;

    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress:
        corner->m_dragStart = mevent->scenePos();
        return true;
    case QEvent::GraphicsSceneMouseRelease:
        corner->m_dragStart = QPointF();
        // TODO: Notify somebody?
        return true;
    case QEvent::GraphicsSceneMouseMove:
    {
        QPointF delta = mevent->scenePos() - corner->m_dragStart;
        delta.rx() *= corner->m_fX;
        delta.ry() *= corner->m_fY;
        QRectF aa = wrappedRect();
        QRectF r = mapToScene(wrappedRect()).boundingRect();
        r.setWidth(r.width() + delta.x());
        r.setHeight(r.height() + delta.y());
        QPointF p = m_wrapped->pos();
        prepareGeometryChange();
        m_wrapped->setDrawingGeometry(r);
        return true;

    }
    }

    return false;
}

void GGSelectionItem::updateCornerGrabberPos()
{
    const qreal w = wrappedRect().width()/2;
    const qreal h = wrappedRect().height()/2;
    const qreal d = GGCornerGrabber::GrabberSize/2;
    for (int i = 0; i < m_grabbers.size(); ++i) {
        m_grabbers[i]->setPos(m_grabbers[i]->m_fX*w+d, m_grabbers[i]->m_fY*h+d);
    }
}

QRectF GGSelectionItem::wrappedRect() const
{
    if (!m_wrapped) return QRectF();
    return m_wrapped->innerBoundingRect();
}
