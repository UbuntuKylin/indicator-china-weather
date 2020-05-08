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

class LeftUpCityBtn : public QWidget
{
    Q_OBJECT

public:
    explicit LeftUpCityBtn(QWidget *parent=0);

signals:
    void sendCurrentCityId(QString id);
    void requestShowCollCityWeather();
    void requestSetCityWeather(QString weather_data);

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

signals:
    void requestSetCityName(QString cityName);
};

#endif // LEFTUPCITYBTN_H
