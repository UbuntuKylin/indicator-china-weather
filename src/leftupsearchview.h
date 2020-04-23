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

public slots:
    void onThreadStart();

protected:
    void mouseReleaseEvent(QMouseEvent *e) override;

signals:
    void requestSetObserveWeather(ObserveWeather observerdata);
    void requestSetForecastWeather(ForecastWeather forecastweather);
    void requestSetLifeStyle(LifeStyle lifestyle);
    void requestSetCityName(QString cityName);
    void responseFailure(int code);
    void requestGetWeatherData(QString id);

private:
    WeatherWorker *m_weatherWorker = nullptr;
    QString m_cityid;
};

#endif // LEFTUPSEARCHVIEW_H
