#ifndef CITYCOLLECTION_H
#define CITYCOLLECTION_H

#include <QWidget>
#include <QMouseEvent>
#include <QRect>
#include <QScreen>
#include <QStandardPaths>
#include <QDebug>
#include <QObject>

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

signals:
    void sendCurrentCityId(QString id);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void onWeatherDataRequest();
    void onWeatherDataReply();

    void on_btnCancel_clicked();

    void onShowCityAddWiget();
    void onHideCityAddWiget();

private:
    Ui::citycollectionwidget *ui;
    QNetworkAccessManager *m_networkManager;
    CityAddition *m_cityaddition;

    void writeCollectedCity(QString cityId);
    QString readCollectedCity();

    void setCurrentCity();
    void setCollectCity();
    void showCollectCity(int x, int y, bool isShowNormal, QString cityId);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;

    int cityNumber;
};

#endif // CITYCOLLECTION_H
