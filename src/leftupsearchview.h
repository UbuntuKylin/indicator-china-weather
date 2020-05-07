#ifndef LEFTUPSEARCHVIEW_H
#define LEFTUPSEARCHVIEW_H

#include <QListView>
#include <QDebug>
#include "data.h"
class WeatherWorker;


class LeftUpSearchView : public QListView
{
    Q_OBJECT
public:
    explicit LeftUpSearchView(QWidget *parent = 0);
    ~LeftUpSearchView();

    void requestWeatherData(QString cityId);

protected:
    void mouseReleaseEvent(QMouseEvent *e) override;

signals:
    void requestSetCityName(QString cityName);
    void requestGetWeatherData(QString id);
    void requestSetNewCityWeather(QString id);

private:
    QString m_cityid;
};

#endif // LEFTUPSEARCHVIEW_H
