#ifndef CITYADDSEARCHVIEW_H
#define CITYADDSEARCHVIEW_H

#include <QListView>

class CityAddSearchView : public QListView
{
    Q_OBJECT
public:
    explicit CityAddSearchView(QWidget *parent = 0);
    ~CityAddSearchView();

protected:
    void mouseReleaseEvent(QMouseEvent *e) override;
};

#endif // CITYADDSEARCHVIEW_H
