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
    void ThemeLeftUpSearchView(QString str);

protected:
    // handle click event of searchview, send signal when mouse release.
    void mouseReleaseEvent(QMouseEvent *e) override;

signals:
    void requestSetCityName(QString cityName); //点击选中列表中某一个城市后发出，通知更改左上角城市名
    //void requestGetWeatherData(QString id); //发送出去用于获取数据并在主界面显示
    void requestSetNewCityWeather(QString id); //发送出去用于获取数据并在主界面显示

private:
    QString m_cityid;
};

#endif // LEFTUPSEARCHVIEW_H
