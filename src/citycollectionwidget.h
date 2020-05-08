#ifndef CITYCOLLECTION_H
#define CITYCOLLECTION_H

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

#include <unistd.h>

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
    void onRequestAddNewCity(QString cityId);
    void onRequestDeleteCity(QString cityId);
    void onChangeCurrentCity(QString cityId);

    void onRequestSetCityWeather(QString weather_data);

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

    void writeCollectedCity(QString cityId);
    QString readCollectedCity();

    void showCollectCity(int x, int y, bool isShowNormal, QString weatherStr);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
    int m_citynumber; //当前收藏城市数量
    bool isAddCity = false;

signals:
    void sendCurrentCityId(QString id);
    void requestShowCollCityWeather();
    void requestChangeWidgetState();

    void threadFinish();
};

#endif // CITYCOLLECTION_H
