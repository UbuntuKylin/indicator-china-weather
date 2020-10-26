#ifndef LEFTUPCITYBTN_H
#define LEFTUPCITYBTN_H

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include "data.h"
#include "citycollectionwidget.h"

class LeftUpCityBtn : public QWidget
{
    Q_OBJECT

public:
    explicit LeftUpCityBtn(QWidget *parent=0);

signals:
    void sendCurrentCityId(QString id); //发送到主界面更新主界面天气
    void requestShowCollCityWeather(); //显示收藏城市列表天气
    void requestSetCityWeather(QString weather_data); //发送出去显示主界面城市天气
    void updatecity();

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_mouseHover = false;
    bool is_open_city_collect_widget = false;

    QHBoxLayout *m_layout = nullptr;
    QLabel *m_addCityBtn = nullptr;
    QLabel *m_addCityLabel = nullptr;

    CityCollectionWidget *m_citycollectionwidget;

signals:
    void requestSetCityName(QString cityName); //在搜索列表中选中一个城市后，左上角城市名需要更改
};

#endif // LEFTUPCITYBTN_H
