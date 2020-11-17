#ifndef CITYCOLLECTION_H
#define CITYCOLLECTION_H

#define CHINAWEATHERDATA "org.china-weather-data.settings"
#define FITTHEMEWINDOW "org.ukui.style"

#include <QWidget>
#include <QMouseEvent>
#include <QRect>
#include <QScreen>
#include <QStandardPaths>
#include <QDebug>
#include <QObject>
#include <QLabel>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QGraphicsDropShadowEffect>

#include <unistd.h>

#include <QGSettings>
#include "data.h"

class CityAddition;
class citycollectionitem;

namespace Ui {
class citycollectionwidget;
}

class CityCollectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CityCollectionWidget(QWidget *parent = nullptr);
    ~CityCollectionWidget();

public slots:
    //add a new collected city in the collected city list.
    void onRequestAddNewCity(QString cityId);

    //delete a collected city from the collected city list.
    void onRequestDeleteCity(QString cityId);

    //change current city after click a city in collected city list
    void onChangeCurrentCity(QString cityId);

    //slot function to set weather data
    void onRequestSetCityWeather(QString weather_data);

    void updatecity();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_btnCancel_clicked();

    void onShowCityAddWiget();
    void onHideCityAddWiget();


private:
    Ui::citycollectionwidget *ui;
    QNetworkAccessManager *m_networkManager= nullptr;
    CityAddition *m_cityaddition= nullptr;
    QLabel *m_tipIcon = nullptr;
    QLabel *m_tipLabel = nullptr;

    //save new city list to configre file
    void writeCollectedCity(QString cityId);
    //get city list data saved in configure before
    QString readCollectedCity();

    /*
    * handle weather data(weatherStr) and create connect about some signal and slot
    * parameter x and y mean position of item
    * parameter isShowNormal mean whether add city item
    * parameter weatherStr is string about current weather
    */
    void showCollectCity(int x, int y, bool isShowNormal, QString weatherStr);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
    int m_citynumber; //number of current collection cities
    bool isAddCity = false; //whether add a new collect city
    bool is_open_city_collect_widget = false;

    // getstting初始化、值获取、 设置getsetting值
    void initGsetting();
    QString getCityList();
    void setCityList(QString str);
    void setThemeStyle();
    QGSettings  *m_pWeatherData= nullptr;
    QGSettings *m_pThemeStyle = nullptr;


signals:
    void sendCurrentCityId(QString id);
    void requestShowCollCityWeather();
    void requestChangeWidgetState();

    void threadFinish();

};

#endif // CITYCOLLECTION_H
