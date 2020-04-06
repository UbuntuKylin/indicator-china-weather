#ifndef CITYADDSEARCHVIEW_H
#define CITYADDSEARCHVIEW_H

#include <QListView>
#include <QDebug>
#include "data.h"


class CityAddSearchView : public QListView
{
    Q_OBJECT
public:
    explicit CityAddSearchView(QWidget *parent = 0);
    ~CityAddSearchView();

protected:
    void mouseReleaseEvent(QMouseEvent *e) override;

signals:
    void requestClearLineEdit();
    void requestAddNewCity(QString cityId);
};

#endif // CITYADDSEARCHVIEW_H
