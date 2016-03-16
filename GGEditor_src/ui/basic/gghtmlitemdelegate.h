#ifndef GGHTMLITEMDELEGATE_H
#define GGHTMLITEMDELEGATE_H

#include <QStyledItemDelegate>

class GGHtmlItemDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // GGHTMLITEMDELEGATE_H
