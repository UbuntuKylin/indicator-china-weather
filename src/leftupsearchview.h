#ifndef LEFTUPSEARCHVIEW_H
#define LEFTUPSEARCHVIEW_H

#include <QListView>
#include <QDebug>

class LeftUpSearchView : public QListView
{
    Q_OBJECT
public:
    explicit LeftUpSearchView(QWidget *parent = 0);
    ~LeftUpSearchView();

protected:
    void mouseReleaseEvent(QMouseEvent *e) override;
};

#endif // LEFTUPSEARCHVIEW_H
