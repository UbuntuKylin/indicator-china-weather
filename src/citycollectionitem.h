#ifndef CITYCOLLECTIONITEM_H
#define CITYCOLLECTIONITEM_H

#include "data.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkConfigurationManager>

#include <QWidget>
#include <QTime>
#include <QThread>

namespace Ui {
class citycollectionitem;
}

class citycollectionitem : public QWidget
{
    Q_OBJECT

public:
    explicit citycollectionitem(QWidget *parent = nullptr);
    ~citycollectionitem();

    /*
    * set different style of item.
    * parameter isShowNormal mean whether collected city or add city item
    * parameter isCurrentCity mean whether current city
    * parameter collCityNum mean current collecdted city number
    */
    void setItemWidgetState(bool isShowNormal, bool isCurrentCity, int collCityNum);

    /*
    * using the observe weather data to set weather of this city item.
    * parameter observeweather is the current weather data
    */
    void setCityWeather(ObserveWeather observeweather);

    // using network interface to get city weather data from internet.
    void setCurrentWeather(QString cityId);

    // set background image according to current weather code.
    QString convertCodeToBackgroud(int code);

    QString m_city_id = "";
    bool is_curr_city = false;
    bool is_normal_item = true;
    int m_collcitynum;

protected:
    // show close icon when mouse cursor hover on collected city item
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    // used to send requestGetWeatherData signal
    void onThreadStart();

    // show close icon when mouse cursor hover on collected city item
    void onWeatherDataRequest(const QString &cityId);

    // handle weather data get from internet
    void onWeatherDataReply();

    // show add collected widget, a search box and listview in this widget
    void on_btnAddCity_clicked();

    // close widgwet
    void on_btnDelete_clicked();

private:
    Ui::citycollectionitem *ui;
    QNetworkAccessManager *m_networkManager = nullptr;

signals:
    void changeCurrentCity(QString cityId);
    void showCityAddWiget();
    void requestDeleteCity(QString cityId);
    void requestGetWeatherData(QString cityId);
    void mThreadFinish();
};

#endif // CITYCOLLECTIONITEM_H
