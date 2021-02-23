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
    void ThemeCitySearchView(QString str);
protected:
    // handle click event of searchview, send signal when mouse release.
    void mouseReleaseEvent(QMouseEvent *e) override;
public slots:
    void dealSearchBoxKeyPress(QString);

signals:
    void requestClearLineEdit(); //set original state after mouse release
    void requestAddNewCity(QString cityId); //send this signal to mainwindow, show new city weather
private:
    void send();
};

#endif // CITYADDSEARCHVIEW_H
