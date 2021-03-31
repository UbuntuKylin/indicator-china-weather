#ifndef ADDCITYACTION_H
#define ADDCITYACTION_H

#include <QWidget>
#include <QAction>
#include <QRect>
#include <QDBusInterface>
#include "data.h"
#include "citycollectionwidget.h"
class AddCityAction : public QAction
{
    Q_OBJECT
public:
    explicit AddCityAction(QWidget *parent = nullptr);
signals:

    void cityNameLabelSignal(QString cityName);
    void sendCurrentCityId(QString id); //发送到主界面更新主界面天气
    void requestShowCollCityWeather(); //显示收藏城市列表天气
    void requestSetCityWeather(QString weather_data); //发送出去显示主界面城市天气
    void updatecity();

//protected:
//    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
//    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
//    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;//需要此功能的信号槽函数
//    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    bool is_open_city_collect_widget = false;//需要
    QLabel *addCityLabel = nullptr;
    CityCollectionWidget *m_citycollectionwidget;//需要
    void onRequestSendDesktopNotify(QString message);

private slots:
    void addCityClick();
signals:

    void noNetWork();//断网
    void requestSetCityName(QString cityName); //需要//在搜索列表中选中一个城市后，左上角城市名需要更改

};

#endif // ADDCITYACTION_H
