#ifndef CITYADDSEARCHDELEGATE_H
#define CITYADDSEARCHDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QStandardItemModel>

class CityAddSearchDelegate : public QStyledItemDelegate
{
    Q_OBJECT
signals:

public:
    explicit CityAddSearchDelegate(QObject *parent = 0);
    ~CityAddSearchDelegate();

    //重写重画函数
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};

#endif // CITYADDSEARCHDELEGATE_H
