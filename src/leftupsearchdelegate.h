#ifndef LEFTUPSEARCHDELEGATE_H
#define LEFTUPSEARCHDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QStandardItemModel>

#include <QMouseEvent>

class LeftUpSearchDelegate : public QStyledItemDelegate
{
    Q_OBJECT
signals:

public:
    explicit LeftUpSearchDelegate(QObject *parent = 0);
    ~LeftUpSearchDelegate();

    //重写重画函数
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};

#endif // LEFTUPSEARCHDELEGATE_H
