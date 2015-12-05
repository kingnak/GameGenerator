#include "ggselectionitem.h"
#include "ggresizableitem.h"
#include <QtGui>
#include <QGraphicsSceneMouseEvent>

class GGSelectionItem::GGCornerGrabber : public QGraphicsItem {
    friend class GGSelectionItem;
    GGCornerGrabber(int pX, int pY, int fW, int fH, int fX, int fY, GGSelectionItem *sel) : QGraphicsItem(sel), m_pX(pX), m_pY(pY), m_fW(fW), m_fH(fH), m_fX(fX), m_fY(fY) {
        installSceneEventFilter(sel);
        if (pX == pY) setCursor(Qt::SizeFDiagCursor);
        else if (pX == -pY) setCursor(Qt::SizeBDiagCursor);
        else if (pY == 0) setCursor(Qt::SizeHorCursor);
        else if (pX == 0) setCursor(Qt::SizeVerCursor);
    }
    int m_pX, m_pY, m_fW, m_fH, m_fX, m_fY;
    static const qreal GrabberSize;
    QPointF m_dragStart;
public:
    QRectF boundingRect() const { return QRectF(-GrabberSize,-GrabberSize,GrabberSize,GrabberSize); }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
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

void GGSelectionItem::setWrappedItem(GGResizableItem *item)
{
    setVisible(false);
    m_wrapped = item;
    if (!m_wrapped) return;
    setPos(m_wrapped->pos());
    setVisible(true);
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
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if (!m_wrapped) return;
    painter->setPen(Qt::blue);
    painter->drawRect(m_wrapped->boundingRect());
}

void GGSelectionItem::init()
{
    m_grabbers << new GGCornerGrabber(-1,-1, 0, 0,+1,+1,this)
               << new GGCornerGrabber( 0,-1, 0, 0, 0,+1,this)
               << new GGCornerGrabber(+1,-1, 1, 0, 0,+1,this)
               << new GGCornerGrabber(-1, 0, 0, 0,+1, 0,this)
               << new GGCornerGrabber(+1, 0, 1, 0, 0, 0,this)
               << new GGCornerGrabber(-1,+1, 0, 1,+1, 0,this)
               << new GGCornerGrabber( 0,+1, 0, 1, 0, 0,this)
               << new GGCornerGrabber(+1,+1, 1, 1, 0, 0,this)
                  ;
}

bool GGSelectionItem::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    if (!m_wrapped) return false;

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
        return true;
    case QEvent::GraphicsSceneMouseMove:
    {
        bool sendsScenePos = m_wrapped->flags().testFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        m_wrapped->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, false);
        QPointF delta = mevent->scenePos() - corner->m_dragStart;
        qreal dw = delta.x() * corner->m_fW;
        qreal dh = delta.y() * corner->m_fH;
        qreal dx = delta.x() * corner->m_fX;
        qreal dy = delta.y() * corner->m_fY;
        QRectF r = mapToScene(wrappedRect()).boundingRect();
        r.adjust(dx,dy,dw,dh);

        prepareGeometryChange();
        m_wrapped->resizeToRect(r);
        setPos(m_wrapped->pos());
        updateCornerGrabberPos();
        corner->m_dragStart = mevent->scenePos();
        m_wrapped->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, sendsScenePos);
        return true;

    }
    default:
        return false;
    }

    return false;
}

void GGSelectionItem::updateCornerGrabberPos()
{
    if (!m_wrapped) return;
    const qreal w = m_wrapped->boundingRect().width()/2;
    const qreal h = m_wrapped->boundingRect().height()/2;
    const qreal d = GGCornerGrabber::GrabberSize/2;
    for (int i = 0; i < m_grabbers.size(); ++i) {
        m_grabbers[i]->setPos(m_grabbers[i]->m_pX*w+d, m_grabbers[i]->m_pY*h+d);
    }
}

QRectF GGSelectionItem::wrappedRect() const
{
    if (!m_wrapped) return QRectF();
    return m_wrapped->resizeRect();
}
