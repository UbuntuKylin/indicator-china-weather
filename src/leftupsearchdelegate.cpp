#include "leftupsearchdelegate.h"
#include "dataitem.h"

#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyle>
#include <QEvent>
#include <QDebug>

LeftUpSearchDelegate::LeftUpSearchDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

LeftUpSearchDelegate::~LeftUpSearchDelegate()
{

}

void LeftUpSearchDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();

        ItemStatus status = (ItemStatus)(index.data(Qt::UserRole).toInt());

        QVariant variant = index.data(Qt::UserRole+1);
        ItemData data = variant.value<ItemData>();

        QStyleOptionViewItem viewOption(option);//用来在视图中画一个item

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth( option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        //QPainterPath画圆角矩形
        const qreal radius = 4;
        QPainterPath path;
        path.moveTo(rect.topRight() - QPointF(radius, 0));
        path.lineTo(rect.topLeft() + QPointF(radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -radius));
        path.lineTo(rect.topRight() + QPointF(0, radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-radius, -0));

        if(option.state.testFlag(QStyle::State_Selected))
        {
            painter->setPen(QPen(Qt::blue));
            painter->setBrush(QColor(255, 255, 255));
            painter->setOpacity(0.3);
            painter->drawPath(path);
        }
        else if(option.state.testFlag(QStyle::State_MouseOver))
        {
            painter->setPen(QPen(Qt::green));
            painter->setBrush(QColor(255, 255, 255));
            painter->setOpacity(0.3);
            painter->drawPath(path);
        }
        else{
            painter->setPen(QPen(Qt::gray));
            painter->setBrush(Qt::NoBrush);
            painter->drawPath(path);
        }

        //绘制数据位置
        QRect NameRect = QRect(rect.left()+8, rect.top(), rect.width()-30, 26);
        QRect circle = QRect(NameRect.right() - 10, rect.top()+10, 10, 10);
        QRect telRect = QRect(rect.left()+8, rect.bottom()-24, rect.width()-10, 20);


        switch (status) {
        case S_RED:
            painter->setBrush(Qt::red);
            painter->setPen(QPen(Qt::red));
            break;
        case S_BLUE:
            painter->setBrush(Qt::blue);
            painter->setPen(QPen(Qt::blue));
            break;
        case S_YELLOW:
            painter->setBrush(Qt::yellow);
            painter->setPen(QPen(Qt::yellow));
            break;
        }

        // painter->drawEllipse(circle);     //画圆圈

        painter->setPen(QPen(Qt::white));
        painter->setFont(QFont("Microsoft YaHei", 16));
        painter->drawText(NameRect,Qt::AlignLeft,data.name); //绘制名字

        painter->setPen(QPen(Qt::white));
        painter->setFont(QFont("Microsoft YaHei", 12));
        painter->drawText(telRect,Qt::AlignLeft,data.tel); //绘制电话

        painter->restore();
    }
}

QSize LeftUpSearchDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(180, 50);
}
