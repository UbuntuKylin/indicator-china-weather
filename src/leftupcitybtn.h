#ifndef LEFTUPCITYBTN_H
#define LEFTUPCITYBTN_H

#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include "data.h"//需要
#include "citycollectionwidget.h"//需要


class LeftUpCityBtn : public QWidget
{
    Q_OBJECT
//此类(左上角按钮)暂时用设置菜单中的添加城市代替。
public:
    explicit LeftUpCityBtn(QWidget *parent=0);

signals:
    void sendCurrentCityId(QString id); //需要//发送到主界面更新主界面天气
    void requestShowCollCityWeather(); //需要//显示收藏城市列表天气
    void requestSetCityWeather(QString weather_data); //需要//发送出去显示主界面城市天气
    void updatecity();//需要

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;//需要此功能的信号槽函数
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_mouseHover = false;


    QHBoxLayout *m_layout = nullptr;
    QLabel *m_addCityBtn = nullptr;
    QLabel *m_addCityLabel = nullptr;
bool is_open_city_collect_widget = false;//需要
    CityCollectionWidget *m_citycollectionwidget;//需要

signals:
    void requestSetCityName(QString cityName); //需要//在搜索列表中选中一个城市后，左上角城市名需要更改
};

#endif // LEFTUPCITYBTN_H
