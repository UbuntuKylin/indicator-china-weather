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

    // set the drop down list style of the searchview widget.
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;

    // set the size of drop down item of the searchview widget.
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};

#endif // LEFTUPSEARCHDELEGATE_H
