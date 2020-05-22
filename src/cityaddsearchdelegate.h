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

    // set the drop down list style of the searchview widget.
    void paint(QPainter * painter,const QStyleOptionViewItem & option,const QModelIndex & index) const;

    // set the size of drop down item of the searchview widget.
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};

#endif // CITYADDSEARCHDELEGATE_H
