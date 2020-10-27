#ifndef DARKCITYADDSEARCHDELEGATE_H
#define DARKCITYADDSEARCHDELEGATE_H

#include <QStyledItemDelegate>
#include <QModelIndex>
#include <QStandardItemModel>

class DarkCityAddSearchDelegate : public QStyledItemDelegate
{
    Q_OBJECT
signals:

public:
    explicit DarkCityAddSearchDelegate(QObject *parent = 0);
    ~DarkCityAddSearchDelegate();

    // set the drop down list style of the searchview widget.
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    // set the size of drop down item of the searchview widget.
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};


#endif // DARKCITYADDSEARCHDELEGATE_H
